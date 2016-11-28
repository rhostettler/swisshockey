/*
  Copyright (C) 2013 Jolla Ltd.
  Contact: Thomas Perl <thomas.perl@jollamobile.com>
  All rights reserved.

  You may use this file under the terms of BSD license as follows:

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the Jolla Ltd nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR
  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

import QtQuick 2.0
import Sailfish.Silica 1.0

CoverBackground {
    id: cover

    Image {
        id: hometeamLogo
        anchors {
            right: parent.horizontalCenter
            rightMargin: 1.5*Theme.horizontalPageMargin
            bottom: parent.verticalCenter
        }
        height: Theme.fontSizeExtraLarge
        width: Theme.fontSizeExtraLarge
        fillMode: Image.PreserveAspectFit
        source: "../../icons/" + gameDetailsData.hometeamId + ".png"
    }

    Label {
        id: vsLabel
        anchors {
            verticalCenter: hometeamLogo.verticalCenter
            horizontalCenter: parent.horizontalCenter
        }
        font.pixelSize: Theme.fontSizeMedium
        color: Theme.primaryColor
        text: typeof(gameDetailsData) != "undefined" ? "vs" : qsTr("No game<br />selected.")
    }

    Image{
        anchors{
            left: parent.horizontalCenter
            leftMargin: 1.5*Theme.horizontalPageMargin
            bottom: parent.verticalCenter
        }
        height: Theme.fontSizeLarge
        width: Theme.fontSizeLarge
        fillMode: Image.PreserveAspectFit
        source: "../../icons/" + gameDetailsData.awayteamId + ".png"
    }

    Label {
        anchors {
            top: parent.verticalCenter
            horizontalCenter: parent.horizontalCenter
        }
        font.pixelSize: Theme.fontSizeLarge
        color: Theme.primaryColor
        text: gameDetailsData.totalScore
    }
}


