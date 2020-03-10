const Application = require('../model/application');

exports.get_all_applications_last_value = async () => {
    _apps = [];
    await Application.find({}, (err, apps) => {
        if (err) {
            console.log(err);
            return next(err);
        }
        console.log("Inside local controller: " + apps);
        _apps = apps;
    });
    return _apps;
};