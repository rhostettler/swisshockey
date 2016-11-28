import QtQuick 2.0
import Sailfish.Silica 1.0

// The selection dialog for choosing the filter
Page {
    SilicaListView{
        id: leagueList
        anchors.fill: parent

        header: PageHeader {
            title: qsTr("Select League")
        }

        model: ListModel {
            ListElement { leagueName: "NL A" }
            ListElement { leagueName: "NL B" }
            ListElement { leagueName: "Cup" }
            ListElement { leagueName: "CHL" }
        }

        delegate: ListItem {
            width: parent.width
            id: leagueDelegate
            //contentHeight: Theme.itemSizeSmall

            Label {
                x: Theme.horizontalPageMargin
                text: leagueName
                anchors.verticalCenter: parent.verticalCenter
                color: leagueDelegate.highlighted ? Theme.highlightColor : Theme.primaryColor
            }
            onClicked: {
                overviewPage.leagueName = leagueName;
                appWindow.leagueChanged(leagueName);
                pageStack.pop();
            }
        }
    }
}
