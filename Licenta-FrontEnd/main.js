var app = new Vue({
    el: '#app',
    data: {
        welcomeMessage: 'Welcome to social media control',
        appsMessage: 'Choose the applications that you want to block',
        websitesMessage: 'Choose the websites or URLs that you want to block',
        apps: [
            { _id: '1', name: 'Messenger' },
            { _id: '2', name: 'WhatsApp' },
            { _id: '3', name: 'Telegram' },
            { _id: '4', name: 'Twitter' },
            { _id: '5', name: 'Slack' }
        ],
        checkedApps: []
    },
    methods: {
        saveApplicationList() {
            for(let app of this.apps) {
                if (this.checkedApps.includes(app.name)) {
                    console.log('Adding app: ' + app.name);
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
                for (let app of response.data)
                    apps.push(app.name);
            });
            return apps;
        }
    },
    mounted() {
        let apps = this.getAllApplications();
        this.checkedApps = apps;
    }
})