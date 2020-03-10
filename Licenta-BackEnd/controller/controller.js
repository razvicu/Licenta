const Application = require('../model/application');
const net = require('net');

exports.create_application = (req, res) => {
    let app = new Application({
        _id: req.body._id,
        name: req.body.name
    });

    app.save((err) => {
        if (err && err.code == 11000) {
            console.log("Application with id " + app._id + " already exists");
            res.send("Application with id " + app._id + " already exists");
            return;
        }
        console.log('Created app with id ' + app._id);
        res.send('Application created successfully!');
    });
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

exports.get_application = (async (_id) => {
    return await Application.findById(_id, (err, app) => {
        console.log('App is : ' + app);
    });
});

exports.get_all_applications = (req, res) => {
    Application.find({}, (err, apps) => {
        if (err) {
            console.log(err);
            return next(err);
        }
        console.log(apps);
        res.send(apps);
    });
}
