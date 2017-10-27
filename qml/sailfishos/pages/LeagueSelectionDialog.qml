import QtQuick 2.0
import Sailfish.Silica 1.0

// The selection dialog for choosing the filter
Page {
    SilicaListView{
        id: leagueListView
        anchors.fill: parent
        model: leagueList

        header: PageHeader {
            title: qsTr("Select League")
        }

        delegate: ListItem {
            width: parent.width
            id: leagueListDelegate
            //contentHeight: Theme.itemSizeSmall

            Label {
                x: Theme.horizontalPageMargin
                text: name
                anchors.verticalCenter: parent.verticalCenter
                color: leagueListDelegate.highlighted ? Theme.highlightColor : Theme.primaryColor
            }
            onClicked: {
                overviewPage.leagueName = name;
                appWindow.leagueChanged(id);
                pageStack.pop();
            }
        }
    }
}
