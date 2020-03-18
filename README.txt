The folder "css","static" and"js" stores templates for the web page. There are used to generate the map on the page. The pictures used are also stored in /static/map folder.

The folder templates stores the html templates used by flask. layout.html for the general UI and map1.html for posting the map. 

The file sss.py controls the web page. Once it is run(by Python), the pages will be run on 127.0.0.1:5000. The directory '127.0.0.1:5000' shows a 'hello world'. The directory '127.0.0.1:5000/map'shows the map.

The file Server_ex.c controls the server. It receives the data and modifies the database.

The file ultson_photo_touch_button_client.c is used for controlling the sensors and sending data to the server. The file Client1_led.c is used for controlling the led light.

To run the whole project, it is needed to install the required environment. The file installer.sh is used to install the environment. However, it is also needed to create an account with username=root, password=adpk0131 in the mySQL and create a table with data. The SQL queries are written in the file Queries.sql.

