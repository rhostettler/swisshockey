import QtQuick 1.1
import com.nokia.meego 1.0

PageStackWindow {
    id: appWindow

    initialPage: overviewPage

    GamesOverviewPage {
        id: overviewPage
    }

    /*ToolBarLayout {
        id: commonTools
        visible: true

        ToolIcon {
            iconId: "toolbar-back"
            visible: (pageStack.depth !== 1)

            onClicked: {
                pageStack.pop();
            }
        }

        /*ToolIcon {
            platformIconId: "toolbar-view-menu"
            anchors.right: (parent === undefined) ? undefined : parent.right
            onClicked: (myMenu.status === DialogStatus.Closed) ? myMenu.open() : myMenu.close()
        }*/
    //}

    /*Menu {
        id: myMenu
        visualParent: pageStack
        MenuLayout {
            MenuItem { text: qsTr("Sample menu item") }
        }
    }*/
}
