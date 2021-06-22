package lin.abcdq.openglestest

import android.Manifest
import android.app.Activity
import android.content.Context
import android.content.Intent
import android.graphics.*
import android.opengl.GLSurfaceView
import android.os.Bundle
import android.util.Log
import android.view.Surface
import android.view.TextureView
import android.widget.Button
import android.widget.ImageView
import android.widget.RelativeLayout
import android.widget.TextView
import androidx.appcompat.app.AppCompatActivity
import lin.abcdq.camera.CameraJni
import lin.abcdq.camera.CameraUse
import lin.abcdq.camera.camera.CameraWrapCall
import java.io.File
import java.io.FileOutputStream

//https://www.jianshu.com/u/d82c936b6b71
class MainActivity : AppCompatActivity() {

    companion object {
        private var mType = 0;//0:textureView 1:opengl 2: opengl split
        fun startActivity(context: Activity) {
            val intent = Intent(context, MainActivity::class.java)
            intent.putExtra("type", mType)
            context.startActivity(intent)
            context.finish()
        }
    }

    private lateinit var mCamera: CameraUse
    private var mRender: CameraJni? = null
    private var mPosition = -1

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        requestPermissions(mPermissions, 100)
        val mButtonNext = findViewById<Button>(R.id.bt_next)
        mButtonNext.setOnClickListener {
            mCamera.close()
            if (mType < 2) mType++
            else mType = 0
            startActivity(this)
        }
        mCamera = CameraUse(this)
        val textView = findViewById<TextView>(R.id.tv_title)
        mType = intent.getIntExtra("type", 0)
        when (mType) {
            0 -> {
                textView.text = "TextureView 渲染"
                initTexture()
            }
            1 -> {
                textView.text = "opengles 渲染"
                initGL(1)
            }
            2 -> {
                textView.text = "opengles 分屏渲染"
                initGL(2)
            }
        }
    }

    private fun initTexture() {
        val mButtonFacing = findViewById<Button>(R.id.bt_facing)
        val mButtonSize = findViewById<Button>(R.id.bt_size)
        val mButtonCapture = findViewById<Button>(R.id.bt_capture)
        val mCaptureImageView = findViewById<ImageView>(R.id.iv_capture)
        val mContainer = findViewById<RelativeLayout>(R.id.rl_surface_container)
        val mTextureView = TextureView(this)
        mContainer.addView(mTextureView)
        mButtonFacing?.setOnClickListener {
            mCamera.switch()
        }
        mButtonSize?.setOnClickListener {
            if (mPosition == -1) {
                for (size in mCamera.getPreviewSizes() ?: return@setOnClickListener) {
                    if (size.width == mCamera.getPreviewSize()?.width && size.height == mCamera.getPreviewSize()?.height) {
                        break
                    }
                    mPosition++
                }
            }
            if (mPosition + 1 >= mCamera.getPreviewSizes()?.size ?: 0 || mPosition < 0) {
                mPosition = 0
            } else {
                mPosition++
            }
            mCamera.resize(
                mCamera.getPreviewSizes()?.get(mPosition) ?: return@setOnClickListener,
                mTextureView
            )
        }
        mButtonCapture?.setOnClickListener {
            mCamera.setCall(object : CameraWrapCall {
                override fun onPreview(byteArray: ByteArray, width: Int, height: Int) {

                }

                override fun onCapture(byteArray: ByteArray, width: Int, height: Int) {
                    val bitmap = BitmapFactory.decodeByteArray(byteArray, 0, byteArray.size)
                    runOnUiThread { mCaptureImageView?.setImageBitmap(bitmap) }
                    mCamera.invalidate()
                }
            })
            mCamera.capture()
        }
        mTextureView.surfaceTextureListener = object : TextureView.SurfaceTextureListener {
            override fun onSurfaceTextureAvailable(surface: SurfaceTexture, w: Int, g: Int) {
                mCamera.open(Surface(surface))
            }

            override fun onSurfaceTextureSizeChanged(surface: SurfaceTexture, w: Int, g: Int) {
            }

            override fun onSurfaceTextureDestroyed(surface: SurfaceTexture): Boolean {
                return false
            }

            override fun onSurfaceTextureUpdated(surface: SurfaceTexture) {
            }
        }
    }

    private fun initGL(type: Int) {
        val mButtonFacing = findViewById<Button>(R.id.bt_facing)
        val mButtonSize = findViewById<Button>(R.id.bt_size)
        val mButtonCapture = findViewById<Button>(R.id.bt_capture)
        val mCaptureImageView = findViewById<ImageView>(R.id.iv_capture)
        mRender = CameraJni()
        mRender?.onInit(type)
        val mContainer = findViewById<RelativeLayout>(R.id.rl_surface_container)
        val mGLSurfaceView = GLSurfaceView(this)
        mContainer.addView(mGLSurfaceView)
        mGLSurfaceView.setEGLContextClientVersion(3)
        mGLSurfaceView.setRenderer(mRender)
        mButtonFacing?.setOnClickListener {
            mCamera.switch()
        }
        mButtonSize?.setOnClickListener {
            if (mPosition == -1) {
                for (size in mCamera.getPreviewSizes() ?: return@setOnClickListener) {
                    if (size.width == mCamera.getPreviewSize()?.width && size.height == mCamera.getPreviewSize()?.height) {
                        break
                    }
                    mPosition++
                }
            }
            if (mPosition + 1 >= mCamera.getPreviewSizes()?.size ?: 0 || mPosition < 0) {
                mPosition = 0
            } else {
                mPosition++
            }
            mCamera.resize(
                mCamera.getPreviewSizes()?.get(mPosition) ?: return@setOnClickListener
            )
        }
        mButtonCapture?.setOnClickListener {
//            mCamera.capture()
            val byteArray = mRender?.onCapture() ?: return@setOnClickListener
            val bitmap = BitmapFactory.decodeByteArray(byteArray, 0, byteArray.size)
            saveBitmap(bitmap)
            runOnUiThread { mCaptureImageView?.setImageBitmap(bitmap) }
        }
        mCamera.setCall(object : CameraWrapCall {
            override fun onPreview(byteArray: ByteArray, width: Int, height: Int) {
                mRender?.onPreview(byteArray, width, height)
            }

            override fun onCapture(byteArray: ByteArray, width: Int, height: Int) {
//                try {
//                    var bitmap = BitmapFactory.decodeByteArray(byteArray, 0, byteArray.size)
//                    val matrix = Matrix()
//                    matrix.postRotate(90f)
//                    bitmap =
//                        Bitmap.createBitmap(bitmap, 0, 0, bitmap.width, bitmap.height, matrix, true)
//                    runOnUiThread { mCaptureImageView?.setImageBitmap(bitmap) }
//                    mCamera.invalidate()
//                } catch (e: Exception) {
//                    e.printStackTrace()
//                }
            }
        })
        mCamera.open()
    }

    private val mPermissions = arrayOf(
        Manifest.permission.WRITE_EXTERNAL_STORAGE,
        Manifest.permission.READ_EXTERNAL_STORAGE,
        Manifest.permission.CAMERA
    )

    fun saveBitmap(bmp: Bitmap): File {
        val appDir = File(this.obbDir, "gl")
        if (!appDir.exists()) {
            appDir.mkdir()
        }
        val fileName = "gltest" + ".jpg"
        val file = File(appDir, fileName)
        try {
            val fos = FileOutputStream(file)
            bmp.compress(Bitmap.CompressFormat.JPEG, 100, fos)
            fos.flush()
            fos.close()
        } catch (e: Exception) {
            e.printStackTrace()
        }
        return file
    }
}