import QtQuick 2.0
import QtQuick.Controls 1.4
import "main.js" as JSControl
TableView {
    TableViewColumn {
        role: "Protocol"
        title: "协议"
    }
    TableViewColumn {
        role: "src_ip"
        title: "源IP"
    }
    TableViewColumn {
        role: "dst_ip"
        title: "目的IP"
    }
    TableViewColumn {
        role: "src_mac"
        title: "源MAC"
    }
    TableViewColumn{
        role: "dst_mac"
        title: "目的MAC"
    }
    TableViewColumn{
        role: "package_length"
        title: "包长度"
    }
    TableViewColumn{
        role: "src_port"
        title: "源端口"
    }
    TableViewColumn{
        role: "dst_port"
        title: "目的端口"
    }

    model: catchModel;

    onDoubleClicked: {
        var dict = catchModel.get(row)
    }
}
