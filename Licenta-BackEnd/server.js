var express = require('express');
const bodyParser = require('body-parser');
const mongoose = require('mongoose');
const Application = require('./route/route');
const net = require('net');
const local_controller = require('./controller/local_controller');
var app = express();

var port = 3000;
let dev_db_url = 'mongodb+srv://admin:admin@cluster0-6undp.mongodb.net/test?retryWrites=true&w=majority';
let mongoDB = process.env.MONGODB_URI || dev_db_url;
mongoose.connect(mongoDB, {useUnifiedTopology: true, 
                           useNewUrlParser: true,
                           useFindAndModify: false });
mongoose.Promise = global.Promise;
let db = mongoose.connection;
db.on('error', console.error.bind(console, 'MongoDB connection error:'));

app.use(bodyParser.json());
app.use(bodyParser.urlencoded({extended: false}));
app.use('/', Application);
app.use((req, res, next) => {
    res.header('Access-Control-Allow-Origin', '*');
    res.header("Access-Control-Allow-Headers", "Origin, X-Requested-With, Content-Type, Accept");
});

app.get('/', (req,res) =>  {
    res.status(200).send('Hello world');
});

app.listen(port, () => {
    console.log('Server running');
})

const server = net.createServer(async (socket) => {
    console.log('Connection from', socket.remoteAddress, 'port', socket.remotePort);
    socket.on('data', async (data) => {
        console.log(data.toString());
        let valz = await local_controller.get_all_applications_last_value();
        console.log(valz);
        let to_send = "";
        for (let val of valz) {
            to_send += val.name + "\n";
        }
        console.log("About to send: " + to_send);
        socket.write(to_send.length > 0 ? to_send : "none");
    });
})
server.listen(3099);