Vue.component('application-area', {
    data: function() {
        return {
            appsMessage: 'Choose the applications that you want to block',
            websitesMessage: 'Choose the websites or URLs that you want to block',
            apps: [
                { _id: '1', name: 'Messenger', timeAllowed: 0, timeUpdated: 0, focused: false },
                { _id: '2', name: 'WhatsApp', timeAllowed: 0, timeUpdated: 0, focused: false },
                { _id: '3', name: 'Telegram', timeAllowed: 0, timeUpdated: 0, focused: false },
                { _id: '4', name: 'Twitter', timeAllowed: 0, timeUpdated: 0, focused: false },
                { _id: '5', name: 'Slack', timeAllowed: 0, timeUpdated: 0, focused: false },
                { _id: '6', name: 'Skype', timeAllowed: 0, timeUpdated: 0, focused: false }
            ],
            checkedApps: [],
            savedMessage: "",
            savedMessageTimeout: 0
        }
    },
    methods: {
        async saveApplicationList() {
            clearInterval(this.savedMessageTimeout);
            this.savedMessage = "";
            let emptyApp = { _id: '-1', name: 'NoApp'};
            let apps_to_add = [];
            let apps_to_delete = [emptyApp];
            for (let app of this.apps) {
                if (this.checkedApps.includes(app.name)) {
                    app.timeUpdated = parseInt(new Date().getTime() / 1000);
                    apps_to_add.push(app);
                }
                else {
                    apps_to_delete.push(app);
                }
                app.focused = false;
            }
            for (let app of apps_to_delete)
                await axios.delete("http://localhost:3000/deleteApplication/" + app._id).then((data) => {
                    console.log(data);
                });
            apps_to_add = apps_to_add.length > 0 ? apps_to_add : [emptyApp];
            axios.post("http://localhost:3000/addApplications", apps_to_add).then((data) => {
                console.log(data);
                this.savedMessage = "Applications saved!";
                this.savedMessageTimeout = setTimeout(() => this.savedMessage = "", 3000);
            })
        },

        getAllApplications() {
            let apps = [];
            let currentTime = parseInt(new Date().getTime() / 1000);
            axios.get("http://localhost:3000/all").then((response) => {
                console.log(response);
                for (let app of response.data) {
                    apps.push(app.name);
                    let diffTime = Math.ceil((app.timeAllowed * 60 - currentTime + app.timeUpdated) / 60);
                    console.log((app.timeAllowed * 60 - currentTime + app.timeUpdated) / 60);
                    this.apps[app._id - 1].timeAllowed = diffTime > 0 ? diffTime : 0;
                    this.apps[app._id - 1].timeUpdated = app.timeUpdated;
                }
            });
            return apps;
        },
        
    },
    mounted() {
        this.checkedApps = this.getAllApplications();
        console.log(this.apps);
        var countdownWorker = new Worker('countdownWorker.js');
        
        // here we update the time for each app every second
        countdownWorker.onmessage = () => {
            let currentTime = parseInt(new Date().getTime() / 1000);
            for (let app of this.apps) {
                if (app.focused || app.timeAllowed == 0 || app.timeUpdated == 0)
                    continue;
                console.log(app.name + " " + app.timeUpdated)
                let diffTime = (currentTime - app.timeUpdated) % 61;
                console.log(diffTime);
                if (diffTime == 0)
                    continue;
                if (diffTime == 60) {
                    console.log('decrementing ' + app.name);
                    app.timeAllowed--;
                }
                console.log("App name " + app.name + " " + app.timeAllowed + "\n");
            }
        } 
    },
    created() {
        window.addEventListener('focus', () => {
            //this.checkedApps = this.getAllApplications();
        });
    },
    destroyed() {
        window.removeEventListener('focus', () => {
            //this.checkedApps = this.getAllApplications();
        });
    },
    template: `<div id="applications">
                <div id="appsMessage">
                {{ appsMessage }}
                </div>
                <div id="allowMessage">
                    allowed:
                </div>
                <ul>
                <li v-for="app in apps">
                    <div>
                    <input :value="app.name" type="checkbox" v-model="checkedApps"> 
                    <span id="name"> {{ app.name }} </span>
                    <input type="number" min="0" max="999" v-model="app.timeAllowed" @focus="app.focused=true"> 
                    <span>mins</span>
                    </div>
                </li>
                </ul>
                <button class="saveButton" v-on:click="saveApplicationList">Save</button>
                <div class="success centered" v-if="savedMessage">{{this.savedMessage}}</div>
               </div>`
});

Vue.component('website-area', {
    data: function() {
        return {
            websitesMessage: 'Choose the websites or URLs that you want to block',
            websites: [],
            savedMessage: "",
            savedMessageTimeout: 0
        }
    },
    methods: {
        async deleteAllWebsites() {
            await axios.delete("http://localhost:3000/deleteAllWebsites");
        },
        async saveWebsitesList() {
            clearInterval(this.savedMessageTimeout);
            this.savedMessage = "";
            this.deleteAllWebsites();
            let emptyMsg = "empty";
            const urlRegex = "([a-zA-Z0-9]*.)*[a-zA-Z]{2,}.[a-zA-Z]{2,}"
            let selectedWebsitesArray = this.websites.length > 0 ? 
                this.websites.split("\n").filter((site) => site.match(urlRegex)) : emptyMsg.split();
            await axios.post("http://localhost:3000/addWebsites", selectedWebsitesArray)
                    .then((data) => {
                        console.log(data);
                        this.savedMessage = "Websites saved!";
                        this.savedMessageTimeout = setTimeout(() => this.savedMessage = "", 3000);
                    });
        },
        async getAllWebsites() {
            this.websites = [];
            await axios.get("http://localhost:3000/allWebsites").then((response) => {
                console.log(response);
                for (let site of response.data) {
                    if (site.str == "empty")
                        continue;
                    this.websites += (site.str + '\n');
                    console.log("Websites: " + this.websites);
                }
            });
        },
        async getPredefinedWebsites() {
            this.websites = [];
            await axios.get("http://localhost:3000/predefinedWebsites").then((response) => {
                console.log(response);
                for (let site of response.data) {
                    this.websites += (site.str + '\n');
                    console.log("Websites: " + this.websites);
                }
            });
        }
    },
    mounted() {
        this.getAllWebsites();
    },
    template: `<div id="websites">
                <div id="websitesMessage">
                {{ websitesMessage }}
                </div>
                <textarea rows="17" cols="30" placeholder="Enter website or URL" v-model="websites"></textarea>
                <div class="centered">
                    <button class="saveButton" v-on:click="saveWebsitesList">Save</button>
                    <button class="loadButton" v-on:click="getPredefinedWebsites">Load predefined</button>
                </div>
                <div class="success centered" v-if="savedMessage">{{this.savedMessage}}</div>
               </div>`

});

var app = new Vue({
    el: '#app',
    data: {
        welcomeMessage: 'Welcome to distractions control',
    }
})