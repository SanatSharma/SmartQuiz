const WebSocket = require('ws');

/*
  Current Implementation Issues:
  1) Center can log in only after teacher has logged in
  2) If teacher goes down, then all the centers associated with the teacher are also killed
  3) If the program crashes, then we'll need some sort of way to start it again


*/

const wss = new WebSocket.Server({ port: 3000 });
console.log("Here");
var teacher_conn = {};
var teachers = {}

wss.on('connection', function connection(ws, req) {
  const ip = req.connection.remoteAddress;
  wss.clients.add(ws)
  ws._socket.setKeepAlive(true);

  console.log("Added client!");

  ws.on("close", function(){
    console.log("removing client " + ws);

    if (ws in teachers){
      console.log("Teacher died, kill all center connections");
      // if teacher connection dies then kill all of its corresponding student connections also
      teacher_conn[teachers[ws]].forEach(function each(client){
        client.close();
      });

      // delete enrty from teachers dict
      delete teachers[ws];
      console.log("Teachers: " + JSON.stringify(teachers));
    }

    wss.clients.delete(ws);
  
  });

  ws.on("message", function ws_message(data){

    if (isJson(data)){
      // passed data is JSON
      var obj = JSON.parse(data);
      if ('type' in obj){
        if (obj.type == "teacher"){
          console.log("Teacher Connection")
          teachers[ws] = obj.session;
          teacher_conn[obj.session] = [];
          console.log(teacher_conn)
        }
        else if (obj.type == "center"){
          console.log("Center Connection");

          // check if the session the center is trying to log
          if (obj.session in teacher_conn){
            teacher_conn[obj.session].push(ws);
            console.log(teacher_conn);
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
          teacher_conn[obj.session].forEach(function each(client){
            client.send(d)
          });
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