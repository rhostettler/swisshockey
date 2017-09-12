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

        // TODO: This needs to be loaded dynamically, really
        model: ListModel {
            ListElement { leagueName: "All"; leagueId: "0" }
            ListElement { leagueName: "National League"; leagueId: "1" }
            ListElement { leagueName: "Swiss League"; leagueId: "2" }
            ListElement { leagueName: "Cup"; leagueId: "81" }
            ListElement { leagueName: "CHL"; leagueId: "90" }
            ListElement { leagueName: "Men's National Team"; leagueId: "8" }
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
                appWindow.leagueChanged(leagueId);
                pageStack.pop();
            }
        }
    }
}
