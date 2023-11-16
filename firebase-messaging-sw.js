importScripts('https://www.gstatic.com/firebasejs/7.14.6/firebase-app.js');
importScripts('https://www.gstatic.com/firebasejs/7.14.6/firebase-messaging.js');

var firebaseConfig = {
    apiKey: 'AIzaSyCCac6HO8_6z6UloSV_YNJwGE-iJjIlrQM',
        authDomain: 'monitoring-web-app-8d4fd.firebaseapp.com',
        databaseURL:
          'https://monitoring-web-app-8d4fd-default-rtdb.asia-southeast1.firebasedatabase.app',
        projectId: 'monitoring-web-app-8d4fd',
        storageBucket: 'monitoring-web-app-8d4fd.appspot.com',
        messagingSenderId: '70994297798',
        appId: '1:70994297798:web:341c9d0e5ad77ca8aee8c7',
};

firebase.initializeApp(firebaseConfig);
const messaging=firebase.messaging();

messaging.getToken(messaging, { vapidKey: '<YOUR_PUBLIC_VAPID_KEY_HERE>' }).then((currentToken) => {
    if (currentToken) {
        console.log('CurrentToken: ', currentToken);
      // Send the token to your server and update the UI if necessary
      // ...
    } else {
      // Show permission request UI
      console.log('No registration token available. Request permission to generate one.');
      // ...
    }
  }).catch((err) => {
    console.log('An error occurred while retrieving token. ', err);
    // ...
  });

messaging.setBackgroundMessageHandler(function (payload) {
    console.log(payload);
    const notification=JSON.parse(payload);
    const notificationOption={
        body:notification.body,
        icon:notification.icon
    };
    return self.registration.showNotification(payload.notification.title,notificationOption);
});

// Fungsi untuk menangani notifikasi dari client side
self.addEventListener('push', function (event) {
    const payload = event.data.json();
    const options = {
        body: payload.notification.body,
        icon: payload.notification.icon,
    };
    event.waitUntil(
        self.registration.showNotification(payload.notification.title, options)
    );
});