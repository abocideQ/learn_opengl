package lin.abcdq.openglestest

import android.Manifest
import android.graphics.SurfaceTexture
import android.os.Bundle
import android.util.Log
import android.view.Surface
import android.view.TextureView
import android.widget.Button
import androidx.appcompat.app.AppCompatActivity
import lin.abcdq.camera.camera.CameraUse

class MainActivity : AppCompatActivity() {

    private var mButtonFacing: Button? = null
    private var mButtonSize: Button? = null
    private var mButtonCapture: Button? = null
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