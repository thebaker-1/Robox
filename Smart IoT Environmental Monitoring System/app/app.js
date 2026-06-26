// Import the functions you need from the SDKs you need
import { initializeApp } from "https://www.gstatic.com/firebasejs/12.15.0/firebase-app.js";
import { getAnalytics } from "https://www.gstatic.com/firebasejs/12.15.0/firebase-analytics.js";
// TODO: Add SDKs for Firebase products that you want to use
// https://firebase.google.com/docs/web/setup#available-libraries

// Your web app's Firebase configuration
// For Firebase JS SDK v7.20.0 and later, measurementId is optional
const firebaseConfig = {
  apiKey: "AIzaSyBeFL7Mqlwgdh7yj3jz7-cwgWDotg1GppA",
  authDomain: "realtime-database-25781.firebaseapp.com",
  databaseURL: "https://realtime-database-25781-default-rtdb.firebaseio.com",
  projectId: "realtime-database-25781",
  storageBucket: "realtime-database-25781.firebasestorage.app",
  messagingSenderId: "1013826889457",
  appId: "1:1013826889457:web:bb6f5af44b89c057062b50",
  measurementId: "G-B7XL6F80ZT"
};

// Initialize Firebase
const app = initializeApp(firebaseConfig);
const analytics = getAnalytics(app);

// Get a reference to the Realtime Database
const database = firebase.database();

// DOM elements
const tempElement = document.getElementById("temp-val");
const humElement = document.getElementById("hum-val");
const gasElement = document.getElementById("gas-val");
const statusIndicator = document.getElementById("status-indicator");

// Reference path in RTDB matching the one uploaded in the Arduino code
const sensorDataRef = database.ref("/sensor_data");

// Attach an asynchronous listener to fetch data in real-time
sensorDataRef.on("value", (snapshot) => {
  const data = snapshot.val();
  if (data) {
    const temp = data.temperature;
    const hum = data.humidity;
    const gas = data.air_quality;

    // Update individual values
    tempElement.textContent = temp !== undefined ? temp.toFixed(1) : "--";
    humElement.textContent = hum !== undefined ? hum : "--";
    gasElement.textContent = gas !== undefined ? gas : "--";

    // Update Status Indicator based on sensor safety thresholds
    updateStatusIndicator(temp, hum, gas);
  }
}, (error) => {
  console.error("Error reading database path:", error);
});

// Update indicators synchronously with hardware logic
function updateStatusIndicator(temp, hum, gas) {
  const isDanger = (gas > 700 || temp > 45.0 || hum > 85);
  const isWarning = (gas > 400 || temp > 35.0 || hum > 70);

  // Clear previous states
  statusIndicator.className = "indicator";

  if (isDanger) {
    statusIndicator.textContent = "DANGER TRIGGERED";
    statusIndicator.classList.add("red");
  } else if (isWarning) {
    statusIndicator.textContent = "WARNING - ABNORMAL LIMITS";
    statusIndicator.classList.add("yellow");
  } else {
    statusIndicator.textContent = "System Normal";
    statusIndicator.classList.add("green");
  }
}