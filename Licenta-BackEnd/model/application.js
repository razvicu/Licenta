const mongoose = require('mongoose');
const Schema = mongoose.Schema;

let applicationSchema = new Schema({
    _id: {type: String, required: true},
    name: {type: String, required: true}
}, {versionKey: false});

module.exports = mongoose.model('Application', applicationSchema);