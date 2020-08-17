// add this file to .gitignore

let scopes = ['notifications', 'user', 'read:org', 'repo'];

module.exports = {
    github: {
        clientID: '40e7c251597ae238bf85',
        clientSecret: '2962fb062f37097b3732d062186612aac8c47a9a',
        scope: scopes.join(' ')
    },
    session: {
        cookieKey: 'thenetninjaisawesomeiguess'
    }
};