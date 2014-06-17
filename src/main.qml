import QtQuick 2.0
import "qrc:/sourceFiles/qml/jbQuick/Charts/"
import "qrc:/sourceFiles/qml/jbQuick/Charts/QChart.js" as Charts
import "qrc:/sourceFiles/qml/jbQuick/Charts/QChartGallery.js" as ChartsData

Rectangle {
    signal startBenchmark
    id: mainQmlWindow


    onStartBenchmark: {
        benchmarkCopy.execute();
    }

    Connections {
        target: benchmarkCopy;
        onBenchmarkComplete: {

            updateChart();
        }

    }

    function updateChart() {
        //madness begins
        //delete layout's children ( chart_bar )
        for(var i = layout.children.length; i > 0 ; i--) {
            //            console.log("destroying: " + i)
            layout.children[i-1].destroy()
        }
        //create a new chart in its place
        Qt.createQmlObject("import QtQuick 2.0;
    import 'qrc:/sourceFiles/qml/jbQuick/Charts/'
    import 'qrc:/sourceFiles/qml/jbQuick/Charts/QChart.js' as Charts
    import 'qrc:/sourceFiles/qml/jbQuick/Charts/QChartGallery.js' as ChartsData


    Chart {
        id: chart_bar;
          width: parent.width;
          height: parent.height;
          chartAnimated: true;
          chartAnimationEasing: Easing.OutBounce;
          chartAnimationDuration: 2000;
          chartData: benchmarkCopy.getJsonData();

          chartType: Charts.ChartType.BAR;
    }",layout,1);
        //madness ends

    }

    property int chart_width: 300;
    property int chart_height: 300;
    property int chart_spacing: 20;
    property int text_height: 80;
    property int row_height: 8;



    color: "#ffffff";
    width: chart_width*3 + 2*chart_spacing;
    height: chart_height*2 + chart_spacing + 2*row_height + text_height;

    // /////////////////////////////////////////////////////////////////
    // Header
    // /////////////////////////////////////////////////////////////////

    Rectangle { color: "#282b36"; width: parent.width/1.0; height: row_height; }
    Rectangle { color: "#f33e6f"; width: parent.width/3.0; height: row_height; x: 0*width; y: height; }
    Rectangle { color: "#46bfbd"; width: parent.width/3.0; height: row_height; x: 1*width; y: height; }
    Rectangle { color: "#fbd45c"; width: parent.width/3.0; height: row_height; x: 2*width; y: height; }

    Text {

        y: 2*row_height;

        width: parent.width;
        height: text_height;

        text: "QChart.js";
        font.pointSize: 32;

        horizontalAlignment: Text.AlignHCenter;
        verticalAlignment: Text.AlignVCenter;

        Rectangle {

            id: button;

            anchors.top:  parent.top;
            anchors.topMargin: (parent.height-parent.font.pointSize)/2;
            anchors.right: parent.right;
            anchors.rightMargin: (parent.height-parent.font.pointSize)/2;

            width: 100;
            height: 32;

            color: "#2d91ea";
            radius: 8;

            Text {
                anchors.centerIn: parent;
                color: "#ffffff";
                text: "Run Benchmark";
                font.bold: true;
            }

            MouseArea {
                anchors.fill: parent;
                onPressed: {
                    button.color = "#1785e6"
                }
                onReleased: {
                    button.color = "#2d91ea"
                    startBenchmark();
                }
            }
        }
    }

    // /////////////////////////////////////////////////////////////////
    // Body
    // /////////////////////////////////////////////////////////////////

    Grid {

        id: layout;

        x: 0;
        y: 2*row_height + text_height;

        width: parent.width;
        height: parent.height - 2*row_height - text_height;

        columns: 1;
        spacing: chart_spacing;

        Component.onCompleted: {
            Qt.createQmlObject("import QtQuick 2.0;
import 'qrc:/sourceFiles/qml/jbQuick/Charts/'
import 'qrc:/sourceFiles/qml/jbQuick/Charts/QChart.js' as Charts
import 'qrc:/sourceFiles/qml/jbQuick/Charts/QChartGallery.js' as ChartsData


Chart {
    id: chart_bar;
      width: parent.width;
      height: parent.height;
      chartAnimated: true;
      chartAnimationEasing: Easing.OutBounce;
      chartAnimationDuration: 2000;
      chartData: ChartsData.ChartBarData;

      chartType: Charts.ChartType.BAR;
}",layout,1);
        }
    }

}
