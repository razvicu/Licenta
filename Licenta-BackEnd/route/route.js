const express = require('express');
const router = express.Router();
const controller = require('../controller/controller');

router.post('/addApplication', controller.add_application);
router.post('/addApplications', controller.add_applications);
router.post('/addWebsite', controller.add_website);
router.post('/addWebsites', controller.add_websites);
router.delete('/delete/:id', controller.delete_application);
router.delete('/deleteApplication/:id', controller.delete_application);
router.delete('/deleteAllWebsites', controller.delete_all_websites);
router.get('/all', controller.get_all_applications);
router.get('/allWebsites', controller.get_all_websites);
router.get('/predefinedWebsites', controller.get_all_predefined_websites);

module.exports = router;
