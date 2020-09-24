const mongoose = require('mongoose');
const Schema = mongoose.Schema;

let siteSchema = new Schema({
    _id: {type: String, required: true},
    str: {type: String, required: true}
}, {versionKey: false});

let website_ = mongoose.model('Website', siteSchema);
let pwebsite_ = mongoose.model('PredefinedWebsite', siteSchema, 'predefinedWebsites');
module.exports = {
    Website: website_,
    PredefinedWebsite: pwebsite_
}