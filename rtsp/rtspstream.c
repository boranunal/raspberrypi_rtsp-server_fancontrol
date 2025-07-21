#include <gst/gst.h>
#include <gst/rtsp-server/rtsp-server.h>

int main(int argc, char *argv[]) {
    gst_init(&argc, &argv);

    GstRTSPServer *server = gst_rtsp_server_new();
    GstRTSPMountPoints *mounts = gst_rtsp_server_get_mount_points(server);
    GstRTSPMediaFactory *factory_cam, *factory_test;

    factory_cam = gst_rtsp_media_factory_new();
    factory_test = gst_rtsp_media_factory_new();
    // Change pipeline strings as needed for your camera or test source
    // For Pi Camera (libcamera)
    const gchar *pipeline_cam = "( libcamerasrc ! video/x-raw,width=640,height=480,framerate=30/1 ! videoconvert ! x264enc tune=zerolatency bitrate=500 speed-preset=superfast ! rtph264pay name=pay0 pt=96 )";
    
    // Test stream
    const gchar *pipeline_test = "( videotestsrc is-live=true ! video/x-raw,width=640,height=480,framerate=30/1 "
    "! x264enc tune=zerolatency bitrate=512 speed-preset=superfast "
    "! rtph264pay name=pay0 pt=96 )";

    gst_rtsp_media_factory_set_launch(factory_cam, pipeline_cam);
    gst_rtsp_media_factory_set_shared(factory_cam, TRUE);
    gst_rtsp_mount_points_add_factory(mounts, "/stream", factory_cam);

    gst_rtsp_media_factory_set_launch(factory_test, pipeline_test);
    gst_rtsp_media_factory_set_shared(factory_test, TRUE);
    gst_rtsp_mount_points_add_factory(mounts, "/test", factory_test);

    g_object_unref(mounts);

    gst_rtsp_server_attach(server, NULL);

    g_print("RTSP camera stream ready at rtsp://<raspberrypi-ip>:8554/stream\n");
    g_print("RTSP test stream ready at rtsp://<raspberrypi-ip>:8554/test\n");
    GMainLoop *loop = g_main_loop_new(NULL, FALSE);
    g_main_loop_run(loop);

    return 0;
}

