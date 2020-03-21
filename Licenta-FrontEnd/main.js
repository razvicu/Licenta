var app = new Vue({
    el: '#app',
    data: {
        welcomeMessage: 'Welcome to social media control',
        appsMessage: 'Choose the applications that you want to block',
        websitesMessage: 'Choose the websites or URLs that you want to block',
        apps: [
            { _id: '1', name: 'Messenger', time: 0 },
            { _id: '2', name: 'WhatsApp', time: 0 },
            { _id: '3', name: 'Telegram', time: 0 },
            { _id: '4', name: 'Twitter', time: 0 },
            { _id: '5', name: 'Slack', time: 0 },
            { _id: '6', name: 'Skype', time: 0 }
        ],
        checkedApps: []
    },
    methods: {
        saveApplicationList() {
            for(let app of this.apps) {
                if (this.checkedApps.includes(app.name)) {
                    console.log('Adding app: ' + app.time);
                    axios.post("http://localhost:3000/create", app)
                            .then((data) => {
                                console.log(data);
                            });
                } else {
                    console.log('Deleting app: ' + app.name);
                    axios.delete("http://localhost:3000/delete/" + app._id).then((data) => {
                        console.log(data);
                    });
                }
            }
        },

        saveWebsitesList() {
            
        },

        getAllApplications() {
            let apps = [];
            axios.get("http://localhost:3000/all").then((response) => {
                console.log(response);
                for (let app of response.data) {
                    apps.push(app.name);
                    this.apps[app._id - 1].time = app.time;
                }
            });
            return apps;
        }
        
    },
    mounted() {
        let apps = this.getAllApplications();
        this.checkedApps = apps;
    }
})