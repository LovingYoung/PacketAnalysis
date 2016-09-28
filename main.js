var allDevsTabIndex = -1
var catchAnalysisTabIndex = -1
var firstCatch = true

function selectDev(){
    if(allDevsTabIndex != -1){
        tabview.removeTab(allDevsTabIndex)
    }
    var component = Qt.createComponent("selectDev.qml")
    tabview.addTab("网卡信息", component)
    allDevsTabIndex = tabview.count - 1
}

function setDevsModel(){
    alldevsModel.clear()
    var alldevs = mediatype.alldevs
    for(var i = 0; i < alldevs.length; i += 2){
        alldevsModel.append({ "Name": alldevs[i], "Description": alldevs[i+1] })
    }
}

function setCatchModel(){
    mediatype.refreshAnalysis()
    var catchs = mediatype.packetAnalysis
    for(var i = 0; i < catchs.length;){
        console.log("Now: " + catchs[i])
        if(catchs[i] == "TCP"){
            mediatype.popAnalysis(14)
            catchModel.append({"Protocol": catchs[i], "src_ip": catchs[i+1], "src_mac":catchs[i+2], "dst_ip": catchs[i+3], "dst_mac": catchs[i+4], "package_length": catchs[i+5], "src_port": catchs[i+6], "dst_port": catchs[i+7], "seq": catchs[i+8], "ack": catchs[i+9], "flags": catchs[i+10], "window_size": catchs[i+11], "checksum": catchs[i+12], "urgent_p": catchs[i+13] })
        } else if (catchs[i] == "UDP"){
            mediatype.popAnalysis(10)
            catchModel.append({"Protocol": catchs[i], "src_ip": catchs[i+1], "src_mac":catchs[i+2], "dst_ip": catchs[i+3], "dst_mac": catchs[i+4], "package_length": catchs[i+5], "src_port": catchs[i+6], "dst_port": catchs[i+7], "len": catchs[i+8], "checksum": catchs[i+9] })
        } else {
            while(true);
        }
    }
}

function catchAnalysisTab(){
    if(catchAnalysisTabIndex!= -1){
        tabview.removeTab(catchAnalysisTabIndex)
    }
    var component = Qt.createComponent("catchTab.qml")
    tabview.addTab("数据包信息", component)
    allDevsTabIndex = tabview.count - 1
}
