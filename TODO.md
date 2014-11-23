NL Live Scores TODO
===================

General
-------
* Package and upload to openrepos
* Make a webpage (toazter.ch)?
* Error handling on timeout
* Implement functionality for NLB
* Catch app close-event and only put to background, keeping updates "alive"
  (configurable)?
* Implement game-details
  => MouseArea in the delegate, slots and signals should be easily handable
  => the basic strucutre is in place in the delegate. Now i need to make a 
     sensible controller in c++ that listens to the onViewChanged signal from 
     the overviewPage, sets the data source of the detailsPage and updates it.
  => Note: I should make it possible that when in details view, the events are 
     also updated
* Implement a callback action for notifications

