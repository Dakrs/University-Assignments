import $ from 'jquery';
import Vue from 'vue';
//import Ipc from '../Ipc';

Vue.component('google-key-modal',{
  props: {
    submit_fun: Function,
  },
  methods: {
    goToURL: function(){
      window.API.Ipc.triggerGOOGLE_URL();
    },
    submit: function(){
      const GOOGLE_KEY = $('#GOOGLE_INPUT_API_KEY').val();
      $('#GOOGLE_INPUT_API_KEY').val('');
      console.log(GOOGLE_KEY);
      if (GOOGLE_KEY.length > 6){
        this.submit_fun(GOOGLE_KEY,1);
      }
      else{
        alert('KEY TOO SHORT');
      }
    }
  },
  template:`
  <div class="modal fade" id="GOOGLE-MODAL" data-focus="true" tabindex="-1" role="dialog" aria-labelledby="exampleModalCenterTitle" aria-hidden="true" style="-webkit-user-select: none;">
    <div class="modal-dialog modal-dialog-centered" role="document">
      <div class="modal-content">
        <div class="modal-header">
          <h5 class="modal-title" id="exampleModalCenterTitle">Google API Key</h5>
        </div>
        <div class="modal-body">
          <p>In order to access and colect usefull data for your application we need to access some sources.</p>
          <span @click="goToURL()" style="text-align: center; cursor: pointer;">Link to generate API Key</span>
        </div>
        <form style="margin-left: 10%; margin-right: 10%;" onsubmit="event.preventDefault()">
          <div class="form-group">
            <label for="GOOGLE_API_KEY">API KEY</label>
            <input type="text" class="form-control" id="GOOGLE_INPUT_API_KEY">
          </div>
        </form>
        <div class="modal-footer">
          <button @click="submit()" type="button" class="btn btn-dark">Send</button>
        </div>
      </div>
    </div>
  </div>
  `
})

Vue.component('outlook-key-modal',{
  props: {
    submit_fun: Function,
  },
  methods: {
    goToURL: function(){
      window.API.Ipc.triggerOUTLOOK_URL();
    },
    submit: function(){
      this.submit_fun('Wele');
    }
  },
  template:`
  <div class="modal fade" id="OUTLOOK-MODAL" data-focus="true" tabindex="-1" role="dialog" aria-labelledby="exampleModalCenterTitle" aria-hidden="true" style="-webkit-user-select: none;">
    <div class="modal-dialog modal-dialog-centered" role="document">
      <div class="modal-content">
        <div class="modal-header">
          <h5 class="modal-title" id="exampleModalCenterTitle">Outlook API Key</h5>
        </div>
        <div class="modal-body">
          <p>In order to access and colect usefull data for your application we need to access some sources.</p>
          <span @click="goToURL()" style="text-align: center; cursor: pointer;">Link to generate API Key</span>
        </div>
        <div class="modal-footer">
          <button @click="submit()" type="button" class="btn btn-dark">Send</button>
        </div>
      </div>
    </div>
  </div>
  `
})

Vue.component('github-key-modal',{
  props: {
    submit_fun: Function,
  },
  methods: {
    goToURL: function(){
      //window.API.Ipc.triggerOUTLOOK_URL();
      window.API.Ipc.triggerGITHUB_URL();
    },
    submit: function(){
      this.submit_fun();
    }
  },
  template:`
  <div class="modal fade" id="GITHUB-MODAL" data-focus="true" tabindex="-1" role="dialog" aria-labelledby="exampleModalCenterTitle" aria-hidden="true" style="-webkit-user-select: none;">
    <div class="modal-dialog modal-dialog-centered" role="document">
      <div class="modal-content">
        <div class="modal-header">
          <h5 class="modal-title" id="exampleModalCenterTitle">Github API Key</h5>
        </div>
        <div class="modal-body">
          <p>In order to access and colect usefull data for your application we need to access some sources.</p>
          <span @click="goToURL()" style="text-align: center; cursor: pointer;">Link to generate API Key</span>
        </div>
        <div class="modal-footer">
          <button @click="submit()" type="button" class="btn btn-dark">Send</button>
        </div>
      </div>
    </div>
  </div>
  `
})

Vue.component('login-modal',{
  data: function(){
    return {
      status: true
    };
  },
  props: {
    submit_fun: Function,
  },
  methods: {
    submit: function(type,email,pass){
      this.submit_fun(type,email,pass);
    },
    getStatus: function (){
      return this.$data.status;
    }
  },
  template:`
  <div class="modal fade" id="LOGIN-MODAL" data-focus="true" tabindex="-1" role="dialog" aria-labelledby="exampleModalCenterTitle" aria-hidden="true" style="-webkit-user-select: none;">
    <div class="modal-dialog modal-dialog-centered" role="document">
      <div class="modal-content">
        <div class="modal-header">
          <div class="container">
            <div v-if="status" class="item active">
              <span v-on:click="status = !status" style="font-size: 1.5em;">Log In</span>
            </div>
            <div v-else class="item">
              <span v-on:click="status = !status" style="font-size: 1.5em;">Log In</span>
            </div>
            <div v-if="!status" class="item active">
              <span v-on:click="status = !status" style="font-size: 1.5em;">Sign In</span>
            </div>
            <div v-else class="item">
              <span v-on:click="status = !status" style="font-size: 1.5em;">Sign In</span>
            </div>
          </div>
        </div>
        <div class="modal-body">
          <div class="containerHorizontal">
            <form id="LOGIN-FORM">
              <input type="email" id="EMAIL_LOGIN" class="form-control" placeholder="Email" aria-label="Email" aria-describedby="basic-addon1" style="margin-bottom: 2%" required>
              <input type="password" id="PWD_LOGIN" class="form-control" placeholder="Password" aria-label="Password" aria-describedby="basic-addon1" required>
              <input v-if="!status" id="PWD_CONF_LOGIN" type="password" class="form-control" placeholder="Confirm Password" aria-label="Confirm Password" aria-describedby="basic-addon1" style="margin-top: 2%" minlength="8" required>
            </form>
          </div>
        </div>
        <div class="modal-footer">
          <button form="LOGIN-FORM" type="submit" style="width: 100%" class="btn btn-dark">Submit</button>
        </div>
      </div>
    </div>
  </div>
  `,
  mounted(){
    var func = this.submit;
    var status = this.getStatus;
    $('#LOGIN-FORM').on('submit', function(e){
      e.preventDefault();

      const email = $('#EMAIL_LOGIN').val();
      const pwd = $('#PWD_LOGIN').val();

      const sts = status();
      if (!sts){
        const pwd_conf = $('#PWD_CONF_LOGIN').val();
        if (pwd_conf !== pwd)
          alert('Passwords don\'t match');
        else{
          func(sts,email,pwd);
        }
      }
      else{
        func(sts,email,pwd);
      }
    });
  }
})
