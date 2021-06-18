package lin.abcdq.openglestest

import android.Manifest
import android.graphics.*
import android.opengl.GLSurfaceView
import android.os.Bundle
import android.util.Log
import android.view.Surface
import android.view.TextureView
import android.widget.Button
import android.widget.ImageView
import androidx.appcompat.app.AppCompatActivity
import lin.abcdq.camera.CameraJni
import lin.abcdq.camera.CameraUse
import lin.abcdq.camera.camera.CameraWrapCall
import java.io.ByteArrayOutputStream


class MainActivity : AppCompatActivity() {


    private val mCamera by lazy { CameraUse(this) }

    private var mSizePosition = -1

    override fun onDestroy() {
        super.onDestroy()
        mCamera.close()
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        requestPermissions(mPermissions, 100)
//        initTexture()
        initGL()
    }

    private fun initTexture() {
        val mButtonFacing = findViewById<Button>(R.id.bt_facing)
        val mButtonSize = findViewById<Button>(R.id.bt_size)
        val mButtonCapture = findViewById<Button>(R.id.bt_capture)
        val mCaptureImageView = findViewById<ImageView>(R.id.iv_capture)
        val mTextureView = findViewById<TextureView>(R.id.tv_surface)
        mButtonFacing?.setOnClickListener {
            mCamera.switch()
        }
        mButtonSize?.setOnClickListener {
            if (mSizePosition == -1) {
                for (size in mCamera.getPreviewSizes() ?: return@setOnClickListener) {
                    if (size.width == mCamera.getPreviewSize()?.width && size.height == mCamera.getPreviewSize()?.height) {
                        break
                    }
                    mSizePosition++
                }
            }
            if (mSizePosition + 1 >= mCamera.getPreviewSizes()?.size ?: 0 || mSizePosition < 0) {
                mSizePosition = 0
            } else {
                mSizePosition++
            }
            mCamera.resize(
                mCamera.getPreviewSizes()?.get(mSizePosition) ?: return@setOnClickListener,
                mTextureView ?: return@setOnClickListener
            )
        }
        mButtonCapture?.setOnClickListener {
            mCamera.setCall(object : CameraWrapCall {
                override fun onPreview(byteArray: ByteArray, width: Int, height: Int) {

                }

                override fun onCapture(byteArray: ByteArray, width: Int, height: Int) {
                    val bitmap = BitmapFactory.decodeByteArray(byteArray, 0, byteArray.size)
                    runOnUiThread { mCaptureImageView?.setImageBitmap(bitmap) }
                }
            })
            mCamera.capture()
        }
        mTextureView?.surfaceTextureListener = object : TextureView.SurfaceTextureListener {
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

    private fun initGL() {
        val mButtonFacing = findViewById<Button>(R.id.bt_facing)
        val mButtonSize = findViewById<Button>(R.id.bt_size)
        val mButtonCapture = findViewById<Button>(R.id.bt_capture)
        val mCaptureImageView = findViewById<ImageView>(R.id.iv_capture)
        mButtonFacing?.setOnClickListener {
            mCamera.switch()
        }
        mButtonCapture?.setOnClickListener {
            mCamera.capture()
        }

        val mRender = CameraJni()
        val mGLSurfaceView = findViewById<GLSurfaceView>(R.id.glsv_surface)
        mGLSurfaceView.setEGLContextClientVersion(3)
        mGLSurfaceView.setRenderer(mRender)
        mCamera.setCall(object : CameraWrapCall {
            override fun onPreview(byteArray: ByteArray, width: Int, height: Int) {
                mRender.preview(byteArray, width, height)
            }

            override fun onCapture(byteArray: ByteArray, width: Int, height: Int) {
                try {
                    val image = YuvImage(byteArray, ImageFormat.NV21, width, height, null)
                    val buffer = ByteArrayOutputStream()
                    image.compressToJpeg(Rect(0, 0, width, height), 100, buffer)
                    val bitmap = BitmapFactory.decodeByteArray(buffer.toByteArray(), 0, buffer.size())
                    runOnUiThread { mCaptureImageView?.setImageBitmap(bitmap) }
                } catch (e: Exception) {
                    e.printStackTrace()
                }
            }
        })
        mCamera.open()
    }

    private val mPermissions = arrayOf(
        Manifest.permission.WRITE_EXTERNAL_STORAGE,
        Manifest.permission.READ_EXTERNAL_STORAGE,
        Manifest.permission.CAMERA
    )
}