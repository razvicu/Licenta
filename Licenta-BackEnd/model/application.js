const mongoose = require('mongoose');
const Schema = mongoose.Schema;

let applicationSchema = new Schema({
    _id: {type: String, required: true},
    name: {type: String, required: true},
    timeAllowed: {type: Number, required: false},
    timeUpdated: {type: Number, required: false},
}, {versionKey: false});

module.exports = mongoose.model('Application', applicationSchema);