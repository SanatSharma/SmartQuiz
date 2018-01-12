const WebSocket = require('ws');

/*
  Current Implementation Issues:
  1) Center can log in only after teacher has logged in
  2) If teacher goes down, then all the centers associated with the teacher are also killed
  3) If the program crashes, then we'll need some sort of way to start it again

*/

const wss = new WebSocket.Server({ port: 3000 });
var teacher_conn = {};
var teachers = {};
var centers = {};

wss.on('connection', function connection(ws, req) {
  const ip = req.connection.remoteAddress;
  wss.clients.add(ws)
  ws._socket.setKeepAlive(true);

  console.log("Added client!");

  ws.on("close", function(){
    console.log("removing client " + ws);

    if (ws in teachers){
    //  console.log("Teacher died, kill all center connections");
      // if teacher connection dies then kill all of its corresponding student connections also
    //  teacher_conn[teachers[ws]].forEach(function each(client){
    //    client.close();
    //  });
      console.log("Teacher disconnection");

      // delete enrty from teachers dict
      delete teachers[ws];
      console.log("Teachers: " + JSON.stringify(teachers));
    }
    else{
      console.log("Center disconnection");
      var a = teacher_conn[centers[ws]];
      var index = a.indexOf(ws);
      a.splice(index, 1);
      console.log(teacher_conn);
    }

    console.log(teacher_conn);
    wss.clients.delete(ws);
  
  });

  ws.on("message", function ws_message(data){

    if (isJson(data)){
      // passed data is JSON
      var obj = JSON.parse(data);
      if ('type' in obj){
        if (obj.type == "teacher"){
          console.log("Teacher Connection")
          if (obj.session in teacher_conn){
            // if corresponding session already exists, just add teacher to the teacher list
            if (ws in teachers){
              // should never reach here. This means that teacher already exists and is trying to make a connection again
              throw "LOGIC ERROR";
            }
            console.log("Session already exists");
            teachers[ws] = obj.session;
            
          }
          else{
            // create the session and add teacher to teacher list
            console.log("New Session");
            teachers[ws] = obj.session;
            teacher_conn[obj.session] = [];
          }
          console.log("Teachers: " + JSON.stringify(teachers));
          console.log(teacher_conn)
        }
        else if (obj.type == "center"){
          console.log("Center Connection");
          if (ws in teachers)
            throw "LOGIC ERROR: Center should not be in teachers dict";
          // check if the session the center is trying to log into exists
          if (obj.session in teacher_conn){
            teacher_conn[obj.session].push(ws);
            console.log(teacher_conn);
            centers[ws] = obj.session;
            console.log(centers);
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
            teacher_conn[obj.session].forEach(function each(client){
              client.close()
            });
          }
          else{
            teacher_conn[obj.session].forEach(function each(client){
              client.send(d)
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