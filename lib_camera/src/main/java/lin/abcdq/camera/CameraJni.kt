package lin.abcdq.camera

import android.opengl.GLSurfaceView
import javax.microedition.khronos.egl.EGLConfig
import javax.microedition.khronos.opengles.GL10

class CameraJni : GLSurfaceView.Renderer {

    companion object {
        const val FORMAT_RGBA = 0x01
        const val FORMAT_NV21 = 0x02
        const val FORMAT_NV12 = 0x03
        const val FORMAT_I420 = 0x04
    }

    init {
        System.loadLibrary("CAMERA_LIB")
    }

    fun preview(dataBytes: ByteArray, w: Int, h: Int) {
        native_OnPreview(dataBytes, w, h)
    }

    override fun onSurfaceCreated(gl: GL10?, config: EGLConfig?) {
        native_OnInit()
    }

    override fun onSurfaceChanged(gl: GL10?, width: Int, height: Int) {
    }

    override fun onDrawFrame(gl: GL10?) {
        native_OnDrawFrame()
    }

    private external fun native_OnInit()

    private external fun native_OnDrawFrame()

    private external fun native_OnPreview(dataBytes: ByteArray, w: Int, h: Int)
}