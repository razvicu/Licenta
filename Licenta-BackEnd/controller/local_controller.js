const Application = require('../model/application');
const Website = require('../model/website').Website;

exports.get_all_applications_last = async () => {
    let _apps = Application.find({});
    console.log("This should happen before broadcasting");
    return _apps;
};

exports.get_all_websites_last = async () => {
    let _websites = Website.find({});
    console.log("This should happen before broadcasting");
    return _websites;
}