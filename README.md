Swiss Ice Hockey
================
An app for Meego Harmattan (Nokia N9) and Sailfish OS that provides live scores
from the Swiss men's national ice hockey leagues, National League A and B.

Roadmap
-------
Version 0.3.0:
* Basic support for Sailfish OS (Jolla Harbour-ready) (w/o notification 
  support and control panel integration)
* Complete renaming from "Live Scores" to "Swiss Ice Hockey"

Future plans
------------
Features:
* Implement a browsable calendar

UI:
* Team logos in game details list
* Implement roster

Backend:
* Code cleanup (yes, it's a mess in some places)
* Implement a GameEvent type system
* Implement an internal data model for game events
* Implement roster

Notifications:
* Notification callback actions (tap opens app)
* Implement a notification-beep when app is in foreground
* (Harmattan) Notifications are not cleared on application shutdown (implement
  in Notifier destructor.)
* Consider notification groups

Settings:
* Implement a custom in-app settings window should be easier to manage

Known Issues
------------
* On Sailfish OS, the harmattan QML-files are shipped as well.

License
-------
This software is free software and licensed under the GNU GPL version 3. The teams' logos included are trademarks of the respective team and used on a fair-use basis.

