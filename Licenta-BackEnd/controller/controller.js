const Application = require('../model/application');
const net = require('net');

exports.create_application = (req, res) => {
    let app = new Application({
        _id: req.body._id,
        name: req.body.name,
        time: req.body.time
    });

    Application.findOneAndUpdate({_id: app._id}, app.toObject(), {upsert: true}, (err, doc) => {
        if (err) {
            console.log("Application with id " + app._id + "could not be created or updated");
            return res.send(500, {error: err});
        }
        console.log('Created or updated app with id ' + app._id);
        return res.send('Application saved successfully!');
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
