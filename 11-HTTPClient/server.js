var express = require('express');
var moment = require('moment');
var app = express();
 
app.get('/', function(req, res) {
  res.send('HTTP Test Server')
});
 
app.use('/watchdog', function (req, res, next) {
  var t = moment.duration(parseInt(req.param('uptime')), 'milliseconds')
  var _message = req.param('ip') + " uptime " + t.hours() + "h " + t.minutes() + "m " + t.seconds() +"s";
  console.log("watchdog from " + _message);
  res.send("ALIVE!!");
});
 
var server = app.listen(9000, function () {
  var host = server.address().address
  var port = server.address().port
  
  console.log("HTTP Server is listening at http://%s:%s", host, port)
})

//app.listen(8080);