import 'bootstrap';
import 'bootstrap/dist/css/bootstrap.min.css';
import Vue from 'vue';

const API = window.API;

async function setURL (){
  var url = await API.GITHUB_URL();
  console.log(url);
  document.getElementById('loadedurl').setAttribute("src",url);
}

setURL();

var api_controller = new Vue({
  el: '#VUE_BUTTON',
  methods:{
    goback: function(){
      API.close_modal();
    }
  }
})
