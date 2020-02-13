const Application = require('../model/application');

exports.test = (req,res) => {
    res.send("Greetings from test controller");
}

exports.create_application = (req, res) => {
    let app = new Application({
        _id: req.body._id,
        name: req.body.name
    });

    app.save(function(err) {
        if (err) {
            console.log(err);
        }
        res.send('Application created successfully!');
    });
}

exports.delete_application = (req, res) => {
    let id = req.params.id;
    console.log(id);
    Application.findOneAndDelete(id, (err, app) => {
        if (err)
            console.log(err);
        console.log("Deleted application with id: " + id);
        res.send("Application with id: " + id + " deleted successfully");
    });
}

exports.get_all_applications = (req, res) => {
    Application.find({}, (err, apps) => {
        if (err) {
            console.log(err);
            return next(err);
        }
        res.send(apps);
    })
}