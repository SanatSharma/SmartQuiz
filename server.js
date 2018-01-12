const WebSocket = require('ws');

/*
  Current Implementation Issues:
  1) Center can log in only after teacher has logged in
  2) If teacher goes down, then all the centers associated with the teacher are also killed
  3) If the program crashes, then we'll need some sort of way to start it again

  teacher: {id: [ws, session]}
  center: {id: [ws, session]}
  teacher_conn = {session: [id, id .....]}

*/

const wss = new WebSocket.Server({ port: 3000 });
var teacher_conn = {};
var teachers = {};
var centers = {};
var idCounter = 0;

wss.on('connection', function connection(ws, req) {
  const ip = req.connection.remoteAddress;
  wss.clients.add(ws)
  ws._socket.setKeepAlive(true);

  console.log("Added client!");

  ws.on("close", function(){
    console.log("removing client " + ws);

    if (ws.id in teachers){
    //  console.log("Teacher died, kill all center connections");
      // if teacher connection dies then kill all of its corresponding student connections also
    //  teacher_conn[teachers[ws]].forEach(function each(client){
    //    client.close();
    //  });
      console.log("Teacher disconnection");
      if( ws.closeSession == true){
        // you have removed other connections. Delete session
        console.log("here");
        var session = teachers[ws.id][1];
        delete teacher_conn[session];
      }
      // delete entry from teachers dict
      delete teachers[ws.id];
      console.log("Teachers: " + teachers);
    }
    else{
      console.log("Center disconnection");
      if (ws.id in centers){
        var session = centers[ws.id][1];
        var a = teacher_conn[session];
        var index = a.indexOf(ws.id);
        if (index == -1){
          throw "Center to delete not showing up in connections";
        }
        a.splice(index, 1);
        delete centers[ws.id];
        console.log("Centers: " + centers);
      }
      else{
        // came here because no session was found to connect into
        // Do nothing
      }
    }
    console.log("Connections: " + JSON.stringify(teacher_conn));
    wss.clients.delete(ws);
  
  });

  ws.on("message", function ws_message(data){

    if (isJson(data)){
      // passed data is JSON
      var obj = JSON.parse(data);
      if ('type' in obj){
        ws.id = idCounter++;
        if (obj.type == "teacher"){
          ws.closeSession = false;
          teachers[ws.id]  = [ws, obj.session];
          console.log("Teacher Connection")
          if (obj.session in teacher_conn){
            // if corresponding session already exists, just add teacher to the teacher list
            if (ws in teachers){
              // should never reach here. This means that teacher already exists and is trying to make a connection again
              throw "LOGIC ERROR";
            }
            console.log("Session already exists");
          }
          else{
            // create the session and add teacher to teacher list
            console.log("New Session");
            teacher_conn[obj.session] = [];
          }
          console.log("Teachers: " + teachers);
          console.log("Connections: " + JSON.stringify(teacher_conn));
        }
        else if (obj.type == "center"){
          console.log("Center Connection");
          if (ws.id in teachers)
            throw "LOGIC ERROR: Center should not be in teachers dict";
          // check if the session the center is trying to log into exists
          if (obj.session in teacher_conn){
            teacher_conn[obj.session].push(ws.id);
            console.log("Connections: " + JSON.stringify(teacher_conn));
            centers[ws.id] = [ws, obj.session];
            console.log("Centers: " + centers);
          }
          else{
            console.log("No corresponding session");
            ws.close();
          }
        }
      }
      else{
        console.log("Data: " + obj.data);
        // Broadcast message to all the centers
        var d = obj.data;
        if (obj.session in teacher_conn){
          if (obj.data == "close"){
            ws.closeSession = true;
            console.log("Close Session");
            teacher_conn[obj.session].forEach(function each(client){
              centers[client][0].close()
            });
          }
          else{
            teacher_conn[obj.session].forEach(function each(client){
              centers[client][0].send(d)
            });
          }
        }
        else{
          console.log("No corresponding session");
          ws.close();
        }

      }
    }
    else{
      console.log("Data not JSON!");
      ws.close();
    }
  });
  // Handle any errors that occur.
  ws.on("error", function(error) {
    console.log('WebSocket Error: ' + error);
  });
});


function isJson(str) {
  try {
      console.log(str);
      JSON.parse(str);
  } catch (e) {
      console.log(e);
      return false;
  }
  return true;
}