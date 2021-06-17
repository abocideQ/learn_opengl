package lin.abcdq.camera.camera

interface CameraWrapCall {
    fun preview(byteArray: ByteArray, width: Int, height: Int)
    fun capture(byteArray: ByteArray, width: Int, height: Int)
}