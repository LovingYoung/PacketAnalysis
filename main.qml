import QtQuick 2.3
import QtQuick.Window 2.2
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.1
import "main.js" as JSControl
import com.liuyang.MediaType 1.0

ApplicationWindow{
    id: window
    visible: true
    width: 1024
    height: 768
    menuBar: MenuBar{
        Menu{
            title: "File"
            MenuItem { text: "Open Dumped File" }
            MenuItem {
                text: "Close"
                onTriggered: {
                }
            }
        }
        Menu {
            title: "Edit"
            MenuItem{
                text: "Show all devices"
                onTriggered: {
                    mediatype.refreshAllDevs()
                    JSControl.selectDev()
                    JSControl.setDevsModel()
                }
            }
        }
    }

    TabView{
        id: tabview
        anchors.fill: parent
    }

    MediaType{
        id: mediatype
    }

    ListModel{
        id: alldevsModel
    }

    ListModel{
        id: catchModel
    }

    Timer{
        id: timer
        interval: 1000
        running: false
        repeat: true
        onTriggered: JSControl.setCatchModel()
    }

    Dialog{
        id: optionDialog
        Component.onCompleted: visible=false
        title: "设置参数"
        width: 400
        height: 300
        property string devname: "eth0"

        contentItem: ColumnLayout{
            spacing: 60
            anchors.fill: parent

            Rectangle {
                width: 500
                RowLayout{
                    Rectangle{
                        height: 50
                        width: 200
                        Label{
                            text: "抓包数:   "
                            anchors.right: parent.right
                        }
                    }
                    Rectangle{
                        height: 50
                        width: 300
                        TextInput{
                            text: "请在此输入(-1表示无限)"
                            id: catchcount
                            anchors.fill: parent
                        }
                    }
                }
            }
            Rectangle {
                width: 500
                RowLayout{
                    Rectangle{
                        height: 50
                        width: 200
                        Label{
                            text: "过滤规则:    "
                            anchors.right: parent.right
                        }
                    }
                    Rectangle{
                        height: 50
                        width: 300
                        TextInput{
                            text: "请在此输入(空表示无过滤)"
                            id: filterstring
                            anchors.fill: parent
                        }
                    }
                }
            }
            Button {
                text :"确定"
                onClicked: {
                    var catch_value = catchcount.text;
                    var filter_value = filterstring.text;
                    mediatype.beginSniff(optionDialog.devname, catch_value, filter_value)
                    JSControl.catchAnalysisTab()
                    timer.running = true
                    optionDialog.close()
                }
            }
        }
    }
}
