package lin.abcdq.camera.camera

import android.graphics.ImageFormat
import android.media.Image
import android.os.Build
import androidx.annotation.RequiresApi

@RequiresApi(Build.VERSION_CODES.LOLLIPOP)
internal class ImageUtils {

    companion object {
        fun YUV_420_888_data(image: Image): ByteArray {
            val imageWidth = image.width
            val imageHeight = image.height
            val planes = image.planes
            val data = ByteArray(
                imageWidth * imageHeight *
                        ImageFormat.getBitsPerPixel(ImageFormat.YUV_420_888) / 8
            )
            var offset = 0
            for (plane in planes.indices) {
                val buffer = planes[plane].buffer
                val rowStride = planes[plane].rowStride
                // Experimentally, U and V planes have |pixelStride| = 2, which
                // essentially means they are packed.
                val pixelStride = planes[plane].pixelStride
                val planeWidth = if ((plane == 0)) imageWidth else imageWidth / 2
                val planeHeight = if ((plane == 0)) imageHeight else imageHeight / 2
                if (pixelStride == 1 && rowStride == planeWidth) {
                    // Copy whole plane from buffer into |data| at once.
                    buffer[data, offset, planeWidth * planeHeight]
                    offset += planeWidth * planeHeight
                } else {
                    // Copy pixels one by one respecting pixelStride and rowStride.
                    val rowData = ByteArray(rowStride)
                    for (row in 0 until planeHeight - 1) {
                        buffer[rowData, 0, rowStride]
                        for (col in 0 until planeWidth) {
                            data[offset++] = rowData[col * pixelStride]
                        }
                    }
                    // Last row is special in some devices and may not contain the full
                    // |rowStride| bytes of data.
                    // See http://developer.android.com/reference/android/media/Image.Plane.html#getBuffer()
                    buffer[rowData, 0, Math.min(rowStride, buffer.remaining())]
                    for (col in 0 until planeWidth) {
                        data[offset++] = rowData[col * pixelStride]
                    }
                }
            }
            return data
        }
    }
}