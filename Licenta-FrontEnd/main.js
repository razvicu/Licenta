var app = new Vue({
    el: '#app',
    data: {
        message: 'Welcome to social media control',
        apps: [
            { _id: '1', name: 'Messenger' },
            { _id: '2', name: 'WhatsApp' },
            { _id: '3', name: 'Chrome' }
        ],
        checkedApps: []
    },
    methods: {
        saveApplicationList() {
            for(let app of this.apps) {
                if (this.checkedApps.includes(app.name)) {
                    console.log(app);
                    axios.post("http://localhost:3000/create", app)
                            .then(function(data) {
                                console.log(data);
                            });
                        }
                    }
        },
        getAllApplications() {
            let apps = [];
            axios.get("http://localhost:3000/all").then((response) => {
                console.log(response);
                for (let app of response.data)
                    apps.push(app.name);
            })
            return apps;
        }
    },
    mounted() {
        let apps = this.getAllApplications();
        this.checkedApps = apps;
    }
})