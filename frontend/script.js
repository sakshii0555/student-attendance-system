function markAttendance() {
    const name = document.getElementById("name").value;
    const roll = document.getElementById("roll").value;
    const date = document.getElementById("date").value;
    const status = document.getElementById("status").value;

    if (!name || !roll || !date) {
        alert("Please fill all fields");
        return;
    }

    const data =
        "name=" + encodeURIComponent(name) +
        "&roll=" + encodeURIComponent(roll) +
        "&date=" + encodeURIComponent(date) +
        "&status=" + encodeURIComponent(status);

    fetch("http://localhost:8080", {
        method: "POST",
        headers: {
            "Content-Type": "text/plain"
        },
        body: data
    })
    .then(response => response.text())
    .then(msg => {
        alert(msg);   // 🔔 THIS SHOULD SHOW NOW
    })
    .catch(err => {
        alert("Server not running or CORS blocked");
        console.error(err);
    });
}
function viewAttendance() {
    window.location.href = "view.html";
}

function goBack() {
    window.location.href = "index.html";
}

function loadAttendance() {
    fetch("http://localhost:8080/view")
        .then(response => response.text())
        .then(data => {
            document.getElementById("records").textContent = data || "No attendance records found.";
        })
        .catch(err => {
            document.getElementById("records").textContent = "Server not running.";
        });
}
