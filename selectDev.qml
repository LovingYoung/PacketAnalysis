import QtQuick 2.0
import QtQuick.Controls 1.4
TableView {
    TableViewColumn {
        role: "Name"
        title: "网卡名字(双击选择)"
        width: 300
    }
    TableViewColumn {
        role: "Description"
        title: "描述"
        width: 300
    }

    model: alldevsModel

    onDoubleClicked:{
        optionDialog.devname = alldevsModel.get(row).Name;
        optionDialog.open()
    }
}
