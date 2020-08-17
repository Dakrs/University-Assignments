import $ from 'jquery';
import Vue from 'vue';
//import Ipc from '../Ipc';

Vue.component('api-modal',{
  props: {
    todo: Object,
  },
  methods:{
    goToURL: function(){
      window.API.Ipc.triggerGOOGLE_URL();
    },
  },
  template: `
  <div class="modal fade" id="API-MODAL" data-backdrop="static" data-focus="true" tabindex="-1" role="dialog" aria-labelledby="exampleModalCenterTitle" aria-hidden="true" style="-webkit-user-select: none;">
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
            <input type="text" class="form-control" id="INPUT_API_KEY">
          </div>
        </form>
        <div class="modal-footer">
          <button id="SUBMIT_API_KEY" type="button" class="btn btn-dark">Send</button>
        </div>
      </div>
    </div>
  </div>
  `
})


async function API_KEY_REQUEST(){
  const USER_KEY = $('#INPUT_API_KEY').val();
  $('#INPUT_API_KEY').val('');
  console.log(USER_KEY);
  if (USER_KEY.length > 6){
    const res = await SAVE_API_KEY(USER_KEY);
    if (res){
      api_controller.has_GOOGLE_API_KEY = true;
    }
    else{
      alert('ERROR');
    }
  }
  else{
    alert('KEY TOO SHORT');
  }
}

var api_controller = new Vue({
  el: '#VUE-MODEL',
  data: {
    has_GOOGLE_API_KEY: API_KEY_EXISTS(),
  },
  watch: {
    has_GOOGLE_API_KEY: function (val) {
      if (val){
        alert('API KEY Validated');
        $('#API-MODAL').modal('hide');
      }
      else {
        $('#API-MODAL').modal('show');
      }
    }
  },
  mounted: function (){
    this.$nextTick(function () {
      if (!this.has_GOOGLE_API_KEY){
        $('#API-MODAL').modal('show');
        document.getElementById('SUBMIT_API_KEY').addEventListener('click',API_KEY_REQUEST);
      }
    })
  }
})

//verifica se é preciso pedir a chave para a api no modal
function API_KEY_EXISTS(){
  //var key = store.get('GOOGLE_API_KEY');
  var key = window.API.getGOOGLE_KEY_STATUS();
  console.log(key);

  return (typeof key !== 'undefined');
}

//guardar a key localmente.
async function SAVE_API_KEY(key){
  const res = await window.API.Ipc.store_google_api_key(key);
  /**
  if (res)
    store.set('GOOGLE_API_KEY',true);*/
  return res;
}
