const express = require('express');
const router = express.Router();
const controller = require('../controller/controller');

router.post('/create', controller.create_application);
router.delete('/delete/:id', controller.delete_application);
router.get('/all', controller.get_all_applications);

module.exports = router;