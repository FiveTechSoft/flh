#include "FiveLinux.ch"

REQUEST DBFCDX

function Main()

   local oWnd, oBrw, cTest := Space( 10 )

   USE customer VIA "DBFCDX"

   DEFINE WINDOW oWnd TITLE "Testing Browses" SIZE 522, 400

   @ 4, 2 BROWSE oBrw OF oWnd ;
      HEADERS "First", "Last", "Street", "City", "State", "Zip" ;
      FIELDS  First, Last, Street, City, State, Zip

   oBrw:nClrPane = { | nRow, lSelected | If( ! lSelected, If( OrdKeyNo() % 2 == 0, CLR_GREEN, CLR_CYAN ), CLR_BLUE ) } 
   oBrw:nClrText = { | nRow, lSelected | If( ! lSelected, If( OrdKeyNo() % 2 == 0, CLR_YELLOW, CLR_BLACK ), CLR_WHITE ) } 

   @ 31, 2 BUTTON "_Ok" OF oWnd ACTION oWnd:End()

   ACTIVATE WINDOW oWnd

return nil
