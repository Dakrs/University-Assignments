const jwt = require('jsonwebtoken');
const Credential = require('./controllers/credentials')


const credentials = {
    client: {
      id: process.env.APP_ID,
      secret: process.env.APP_PASSWORD,
    },
    auth: {
      tokenHost: 'https://login.microsoftonline.com',
      authorizePath: 'common/oauth2/v2.0/authorize',
      tokenPath: 'common/oauth2/v2.0/token'
    }
  };

  const oauth2 = require('simple-oauth2').create(credentials);

    
function getAuthUrl() {
  const returnVal = oauth2.authorizationCode.authorizeURL({
    redirect_uri: process.env.REDIRECT_URI,
    scope: process.env.APP_SCOPES
  });
  console.log(`Generated auth url: ${returnVal}`);
  return returnVal;
}
  
  exports.getAuthUrl = getAuthUrl;


  
  async function getTokenFromCode(auth_code) {
    var creden = {} 
    var tokenAux = {}
    let result = await oauth2.authorizationCode.getToken({
      code: auth_code,
      redirect_uri: process.env.REDIRECT_URI,
      scope: process.env.APP_SCOPES
    });
  
    const token = oauth2.accessToken.create(result);
    
    creden.type = "OUTLOOK"
    creden.owner ="me"
    tokenAux.access_token = token.token.access_token
    tokenAux.refresh_token =token.token.refresh_token
    tokenAux.scope = token.token.scope
    tokenAux.token_type=token.token.token_type
    tokenAux.expiry_date = token.token.expires_at.getTime()
    creden.token = tokenAux

     await Credential.insert(creden)

  
    return token.token.access_token;
  }
  exports.getTokenFromCode = getTokenFromCode;

  async function getAccessToken() {
    // Do we have an access token cached?
    var token = await Credential.get("OUTLOOK")

   
  
    if (token.length>0) {
      
      // We have a token, but is it expired?
      // Expire 5 minutes early to account for clock differences
      var date =  new Date()
      const expiration = token[0].token.expiry_date - date.getTime();
      if (expiration > 0) {
        // Token is still good, just return it
        return token[0].token.access_token;
      }
      
  
    // Either no token or it's expired, do we have a
    // refresh token?
    const refresh_token = token[0].token.refresh_token
    if (refresh_token) {
      const newToken = await oauth2.accessToken.create({refresh_token: refresh_token}).refresh();
      var tokenAux = {}
      tokenAux.access_token = newToken.token.access_token
      tokenAux.refresh_token =newToken.token.refresh_token
      tokenAux.scope = newToken.token.scope
      tokenAux.token_type=newToken.token.token_type
      tokenAux.expiry_date = newToken.token.expires_at.getTime()
      Credential.update("OUTLOOK",tokenAux)
      return newToken.token.access_token;
    }
  }
  
    // Nothing in the cookies that helps, return empty
    return null;
  }
  
  exports.getAccessToken = getAccessToken;

  
