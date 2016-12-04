Swiss Ice Hockey
================
An app for Meego Harmattan (Nokia N9) and Sailfish OS that provides live scores
from the Swiss men's national ice hockey leagues, National League A and B.

[![Flattr this](http://api.flattr.com/button/flattr-badge-large.png)](https://flattr.com/submit/auto?fid=nw6llg&url=https://github.com/rhostettler/swisshockey)

Roadmap
-------
Version 0.4.0:
* Automatically fetch the results from the last game day if there are no games
  today.

Future plans
------------
Features:
* Standings
* Roster
* Last / next
* Browseable calendar

UI:
* [All] Team logos in game details list
* [SailfishOS] Tablet UI

Backend:
* Code cleanup (yes, it's a mess)
* Implement the GameEvent type system / game event data model

Notifications:
* [Harmattan] Notification callback actions (tap opens app)
* [Harmattan] Implement a notification-beep when app is in foreground
* [Harmattan] Notifications are not cleared on application shutdown (implement
  in Notifier destructor.)
* [Harmattan] Consider notification groups
* [SailfishOS] Implement some kind of notifications

Settings:
* [All] Implement a custom in-app settings window should be easier to manage

Known Issues
------------
* [SailfishOS] Update is sometimes not triggered when minimized.
* [All] GK & shootout events are not implemented.

License
-------
This software is free software and licensed under the GNU GPL version 3. The 
teams' logos included are trademarks of the respective team and used on a 
fair-use basis.


