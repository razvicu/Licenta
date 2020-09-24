var express = require('express');
const bodyParser = require('body-parser');
const mongoose = require('mongoose');
const Application = require('./route/route');
const local_controller = require('./controller/local_controller');
var app = express();
var cors = require('cors');

const WS_PORT = 3098;
const HTTP_PORT = 3000;

var io = require('socket.io')(WS_PORT);

let dev_db_url = 'mongodb+srv://admin:admin@cluster0-6undp.mongodb.net/test?retryWrites=true&w=majority';
let mongoDB = process.env.MONGODB_URI || dev_db_url;
mongoose.connect(mongoDB, {useUnifiedTopology: true, 
                           useNewUrlParser: true,
                           useFindAndModify: false });
mongoose.Promise = global.Promise;
let db = mongoose.connection;
db.on('error', console.error.bind(console, 'MongoDB connection error:'));

sockets = {};

app.use(bodyParser.json());
app.use(bodyParser.urlencoded({extended: false}));
app.use((req, res, next) => {
    req.sockets = sockets;
    res.header('Access-Control-Allow-Origin', '*');
    res.header('Access-Control-Allow-Methods', 'GET,PUT,POST,DELETE');
    res.header("Access-Control-Allow-Headers", "Origin, X-Requested-With, Content-Type, Accept");   
    next();
});
app.use(cors({origin: true}));

app.use('/', Application);

app.get('/', (req,res) =>  {
    res.status(200).send('Hello world');
});

app.listen(HTTP_PORT, () => {
    console.log('Server running');
});

io.on('connection', async (socket) => {
    console.log('socketo connectado');
    let apps = await local_controller.get_all_applications_last();
    let websites = await local_controller.get_all_websites_last();
    sockets[socket.id] = socket;
    socket.emit('appsEmit', apps.length > 0 ? apps : JSON.stringify("[]"));
    socket.emit('websitesEmit', websites.length > 0 ? websites : JSON.stringify("[]"));
    
    /*socket.on('updatedApps', async (data) => {
        console.log("update apps happened " + data);
        console.log("Clients " + Object.keys(io.sockets.sockets));
        setTimeout(async () => {
            let apps = await local_controller.get_all_applications_last();
            console.log("Broadcasting all applications to clients");
            socket.broadcast.emit('appsEmit', apps.length > 0 ? apps : JSON.stringify("[]")); 
            console.log("Emmited apps: " + apps);
        }, 2000);
    });

    socket.on('updatedWebsites', async (data) => {
        console.log("update websites happened " + data);
        console.log("Clients " + Object.keys(io.sockets.sockets));
        setTimeout(async () => {
            let websites = await local_controller.get_all_websites_last();
            console.log("Broadcasting all websites to clients");
            socket.broadcast.emit('websitesEmit', websites.length > 0 ? websites : JSON.stringify("[]"));
            console.log("Emmited websites: " + websites);
        }, 2000);
    });*/
    socket.on('disconnect', (reason) => {
        console.log("Socket disconnected, reason: " + reason);
        delete sockets[socket.id];
    })
});  

