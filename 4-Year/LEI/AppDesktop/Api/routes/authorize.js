var express = require('express');
var router = express.Router();
var authHelper = require('../authOutlook');



router.get('/', async function(req, res, next) {
    // Get auth code
    const code = req.query.code;
  
    // If code is present, use it
    if (code) {
      try {
        await authHelper.getTokenFromCode(code);
        // Redirect to home
        res.jsonp("Close the window")
      } catch (err) {
        res.jsonp(false)
      }
    } else {
      // Otherwise complain
      res.jsonp(false)
    }
  });



module.exports = router;