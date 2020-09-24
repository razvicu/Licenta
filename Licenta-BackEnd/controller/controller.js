const Application = require('../model/application');
const Website = require('../model/website').Website;
const PredefinedWebsite = require('../model/website').PredefinedWebsite;
const local_controller = require('./local_controller');

exports.add_application = (req, res) => {
    let app = new Application({
        _id: req.body._id,
        name: req.body.name,
        timeAllowed: req.body.timeAllowed,
        timeUpdated: req.body.timeUpdated
    });

    Application.findOneAndUpdate({_id: app._id}, app.toObject(), {upsert: true}, (err, doc) => {
        if (err) {
            console.log("Application with id " + app._id + " could not be created or updated");
            return res.send(500, {error: err});
        }
        console.log('Created or updated app with id ' + app._id);
        return res.send('Application saved successfully!');
    });
}

exports.add_applications = async (req, res) => {
    for (let app_data of req.body) {
        let app = new Application({
            _id: app_data._id,
            name: app_data.name,
            timeAllowed: app_data.timeAllowed,
            timeUpdated: app_data.timeUpdated
        });

        await Application.findOneAndUpdate({_id: app._id}, app.toObject(), {upsert: true});
    }
    let applications = await local_controller.get_all_applications_last();
    console.log("Broadcasting " + applications + " to sockets " + req.sockets);
    for (var id in req.sockets)
        req.sockets[id].emit('appsEmit', applications.length > 0 ? applications : JSON.stringify("[]"));
    return res.send('Applications saved succesfully!');
}

exports.add_website = (req, res) => {
    let siteObj = new Website({
        str: req.body.str
    });

    Website.findOneAndUpdate({str: siteObj.str}, siteObj.toObject(), {upsert: true}, (err, doc) => {
        if (err) {
            console.log("Website " + siteObj.str + " could not be saved");
            return res.send(500, {error: err});
        }
        console.log('Created website ' + siteObj.str);

    });
    return res.send('Website saved succesfully!');
}

exports.add_websites = async (req, res) => {
    console.log(req.body);
    let websites_ = [];
    let id_ = 0;
    for (let string of req.body) {
        console.log(string);
        let siteObj = new Website({
            _id: id_++,
            str: string
        });
        websites_.push(siteObj);
    }
    await Website.insertMany(websites_);
    let websites = await local_controller.get_all_websites_last();
    for (var id in req.sockets)
        req.sockets[id].emit('websitesEmit', websites.length > 0 ? websites : JSON.stringify("[]"));
    return res.send('Websites saved succesfully!');
}

exports.delete_application = (req, res) => {
    let id = req.params.id;
    Application.findByIdAndDelete(id, (err, app) => {
        if (err)
            console.log(err);
        if (app == null) {
            console.log("Application with id: " + id + " does not exist");
            res.send("Application with id: " + id + " does not exist");
            return;
        }
        console.log("Deleted application with id: " + app.id);
        res.send("Application with id: " + app.id + " deleted successfully");
    });
}

exports.delete_all_websites = (req, res) => {
    Website.collection.deleteMany({});
    console.log("Deleted all websites");
    res.send("Deleted all websites");
}

exports.get_application = (async (_id) => {
    return await Application.findById(_id, (err, app) => {
        console.log('App is : ' + app);
    });
});

exports.get_all_applications = (_, res) => {
    Application.find({}, (err, apps) => {
        if (err) {
            console.log(err);
            return res.send(500, {error: err});
        }
        console.log(apps);
        res.send(apps);
    });
}

exports.get_all_websites = (_, res) => {
    Website.find({}, (err, websites) => {
        if (err) {
            console.log(err);
            return res.send(500, {error: err});
        }
        console.log(websites);
        res.send(websites);
    });
}

exports.get_all_predefined_websites = (_, res) => {
    PredefinedWebsite.find({}, (err, websites) => {
        if (err) {
            console.log(err);
            return res.send(500, {error: err});
        }
        console.log(websites);
        res.send(websites);
    });
}