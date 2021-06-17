package lin.abcdq.openglestest

import android.Manifest
import android.graphics.Bitmap
import android.graphics.BitmapFactory
import android.graphics.SurfaceTexture
import android.os.Bundle
import android.view.Surface
import android.view.TextureView
import android.widget.Button
import android.widget.ImageView
import androidx.appcompat.app.AppCompatActivity
import lin.abcdq.camera.camera.CameraUse
import lin.abcdq.camera.camera.CameraWrapCall
import java.nio.ByteBuffer


class MainActivity : AppCompatActivity() {

    private var mButtonFacing: Button? = null
    private var mButtonSize: Button? = null
    private var mButtonCapture: Button? = null
    private var mCaptureImageView: ImageView? = null
    private var mTextureView: TextureView? = null
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
        mButtonFacing = findViewById(R.id.bt_facing)
        mButtonSize = findViewById(R.id.bt_size)
        mButtonCapture = findViewById(R.id.bt_capture)
        mCaptureImageView = findViewById(R.id.iv_capture)
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
                override fun preview(byteArray: ByteArray, width: Int, height: Int) {

                }

                override fun capture(byteArray: ByteArray, width: Int, height: Int) {
                    val bitmap = BitmapFactory.decodeByteArray(byteArray, 0, byteArray.size)
                    runOnUiThread {  mCaptureImageView?.setImageBitmap(bitmap) }
                }
            })
            mCamera.capture()
        }
        mTextureView = findViewById(R.id.tv_surface)
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

    private val mPermissions = arrayOf(
        Manifest.permission.WRITE_EXTERNAL_STORAGE,
        Manifest.permission.READ_EXTERNAL_STORAGE,
        Manifest.permission.CAMERA
    )
}