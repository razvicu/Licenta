var express = require('express');
const bodyParser = require('body-parser');
const mongoose = require('mongoose');
const Application = require('./route/route');

var app = express();

var PORT = 3000;
let dev_db_url = 'mongodb+srv://admin:admin@cluster0-6undp.mongodb.net/test?retryWrites=true&w=majority';
let mongoDB = process.env.MONGODB_URI || dev_db_url;
mongoose.connect(mongoDB);
mongoose.Promise = global.Promise;
let db = mongoose.connection;
db.on('error', console.error.bind(console, 'MongoDB connection error:'));

app.use(bodyParser.json());
app.use(bodyParser.urlencoded({extended: false}));
app.use('/', Application);
app.use(function(req, res, next) {
    res.header('Access-Control-Allow-Origin', '*');
    res.header("Access-Control-Allow-Headers", "Origin, X-Requested-With, Content-Type, Accept");
});

app.get('/', function(req,res) {
    res.status(200).send('Hello world');
});

app.listen(PORT, function() {
    console.log('Server running');
})