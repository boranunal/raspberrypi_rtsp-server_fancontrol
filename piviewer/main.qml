import QtQuick 2.15
import QtQuick.Window 2.15
import QtMultimedia 5.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Window {
    visible: true
    width: 1280
    height: 720
    minimumWidth: 320
    minimumHeight: 180

    title: qsTr("RTSP Viewer with qmlglsink")

    property real aspectRatio: 16 / 9
    property bool resizing: false


    onHeightChanged: {
       if (!resizing) {
           resizing = true
           width = height * aspectRatio
           resizing = false
        }
    }

    onWidthChanged: {
        if (!resizing) {
            resizing = true
            height = width / aspectRatio
            resizing = false
        }
    }

    VideoOutput {
        id: stream
        anchors.left: parent.left
        height: parent.height
        width: height/480 * 640
        source: mediaPlayer
    }

    MediaPlayer {
        id: mediaPlayer
        autoPlay: true
        source: "rtsp://10.3.5.12:8554/stream"
        // Uses GStreamer qmlglsink internally automatically
    }

    Timer {
        interval: 2000
        running: true
        repeat: true
        onTriggered: tcpClient.requestUpdate()
    }

    Canvas {
        id: thermometer
        anchors.right: parent.right
        anchors.left: stream.right
        anchors.top: parent.top
        width: parent.width - stream.width
        height: width
        property real temperature: tcpClient.temperature
        property real minTemp: 0
        property real maxTemp: 100
        onPaint: {
            var ctx = getContext("2d");
            ctx.clearRect(0, 0, width, height);

            var bulbRadius = 25;
            var tubeWidth = 20;
            var margin = 10;

            var tubeHeight = height - bulbRadius * 2 - margin;
            var tubeX = (width - tubeWidth) / 2;
            var tubeY = margin;

            // Draw thermometer outline
            ctx.beginPath();
            ctx.fillStyle = "#ccc";
            ctx.fillRect(tubeX, tubeY, tubeWidth, tubeHeight);

            ctx.beginPath();
            ctx.arc(width / 2, height - bulbRadius, bulbRadius, 0, 2 * Math.PI);
            ctx.fill();

            // Fill based on temperature
            var clampedTemp = Math.max(minTemp, Math.min(maxTemp, temperature));
            var percent = (clampedTemp - minTemp) / (maxTemp - minTemp);
            var fillHeight = percent * tubeHeight;

            // Draw mercury
            ctx.fillStyle = "#e53935";  // red
            ctx.fillRect(tubeX, tubeY + tubeHeight - fillHeight, tubeWidth, fillHeight);
            ctx.beginPath();
            ctx.arc(width / 2, height - bulbRadius, bulbRadius - 4, 0, 2 * Math.PI);
            ctx.fill();

            // Draw temperature text
            ctx.font = "50px serif";
            ctx.fillStyle = "#333";
            ctx.textAlign = "center";
            ctx.textBaseline = "middle";
            ctx.fillText(clampedTemp.toFixed(1) + " °C", width/2, height/2);
        }
        onTemperatureChanged: requestPaint()
    }

    Canvas {
        id: fanSpeedCanvas
        anchors.right: parent.right
        anchors.rightMargin: (parent.width - stream.width)/10
        anchors.bottom: parent.bottom
        anchors.bottomMargin: (parent.width - stream.width)/10
        width: parent.width - stream.width - (parent.width - stream.width)/5
        height: width
        property real value: tcpClient.dutyCycle

        onPaint: {
            var ctx = getContext("2d");

            var w = width;
            var h = height;

            ctx.clearRect(0, 0, w, h);

            var centerX = w / 2;
            var centerY = h / 2;
            var radius = Math.min(w, h) / 2 - 10;
            var startAngle = -Math.PI / 2;
            var endAngle = startAngle + (2 * Math.PI) * (value / 100);

            // Draw background circle
            ctx.beginPath();
            ctx.arc(centerX, centerY, radius, 0, 2 * Math.PI);
            ctx.lineWidth = 20;
            ctx.strokeStyle = '#ddd';
            ctx.stroke();

            // Draw progress arc
            ctx.beginPath();
            ctx.arc(centerX, centerY, radius, startAngle, endAngle);
            ctx.lineWidth = 20;
            ctx.strokeStyle = 'blue';
            ctx.stroke();

            // Draw text
            ctx.fillStyle = 'black';
            ctx.font = "50px serif";
            ctx.textAlign = "center";
            ctx.textBaseline = "middle";
            ctx.fillText(value.toFixed(0) + "%", centerX, centerY);
        }

        onValueChanged: requestPaint()
    }
}

