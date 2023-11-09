const loginElement = document.querySelector('#login-form');
const contentElement = document.querySelector('#content-sign-in');
const userDetailsElement = document.querySelector('#user-details');
const authBarElement = document.querySelector('#authentication-bar');

// Elements for sensor readings
const tmpElement = document.getElementById('tmp');
const lvlElement = document.getElementById('lvl');
const flwElement = document.getElementById('flw');
const turElement = document.getElementById('tur');
const autElement = document.getElementById('aut');

// MANAGE LOGIN/LOGOUT UI
const setupUI = (user) => {
  if (user) {
    //toggle UI elements
    loginElement.style.display = 'none';
    contentElement.style.display = 'block';
    authBarElement.style.display = 'block';
    userDetailsElement.style.display = 'block';
    userDetailsElement.innerHTML = user.email;

    // get user UID to get data from database
    var uid = user.uid;
    console.log(uid);

    // Database paths (with user UID)
    var dbPathTmp = 'UsersData/' + uid.toString() + '/temperature';
    var dbPathLvl = 'UsersData/' + uid.toString() + '/water-level';
    var dbPathFlw = 'UsersData/' + uid.toString() + '/water-flow';
    var dbPathTur = 'UsersData/' + uid.toString() + '/turbidity';
    var dbPathAut = 'UsersData/' + uid.toString() + '/automation';

    // Database references
    var dbRefTmp = firebase.database().ref().child(dbPathTmp);
    var dbRefLvl = firebase.database().ref().child(dbPathLvl);
    var dbRefFlw = firebase.database().ref().child(dbPathFlw);
    var dbRefTur = firebase.database().ref().child(dbPathTur);
    var dbRefAut = firebase.database().ref().child(dbPathAut);

    // Update page with new readings
    dbRefTmp.on('value', (snap) => {
      tmpElement.innerText = snap.val().toFixed(2);
    });

    dbRefLvl.on('value', (snap) => {
      lvlElement.innerText = snap.val().toFixed(2);
    });

    dbRefFlw.on('value', (snap) => {
      flwElement.innerText = snap.val().toFixed(2);
    });

    dbRefTur.on('value', (snap) => {
      turElement.innerText = snap.val().toFixed(2);
    });

    dbRefAut.on('value', (snap) => {
      autElement.innerText = snap.val().toFixed(2);
    });

    // if user is logged out
  } else {
    // toggle UI elements
    loginElement.style.display = 'block';
    authBarElement.style.display = 'none';
    userDetailsElement.style.display = 'none';
    contentElement.style.display = 'none';
  }
};
