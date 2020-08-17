import $ from 'jquery';
window.$ = $;
//require('bootstrap/js/dist/modal');
import 'bootstrap';
import 'bootstrap/dist/css/bootstrap.min.css';
import './index.css';
import Vue from 'vue';
import Sortable from 'sortablejs';
import './Components/todoinfo.js';
//import './Components/apimodal.js';
import './Components/modal.js';
import {  sortByOrigin,
          sortByDate,
          sortByNormal,
          updateIndex
        } from './Sorting';
//import Ipc from './Ipc';
const Ipc = window.API.Ipc;
const API = window.API;

/**
Todo:
id: 1,
date: new Date(2017,1,1), !!!Pode ser null
name: 'Aula PSD',
origin: 'Outlook',
description: 'Aula que acontece todas as segundas feiras e é preciso aparecer para poder aprender.', !!! Pode ser null
priority: 2,
index:, !!!Pode ser null
*/

Vue.component('add-todo',{
  props: {
    callback: Function,
  },
  template: `
  <div class="modal fade" id="ADD_TOO_COMP" tabindex="-1" role="dialog" aria-labelledby="exampleModalCenterTitle" aria-hidden="true">
    <div class="modal-dialog modal-dialog-centered" role="document">
      <div class="modal-content">
        <div class="modal-header">
          <h5 class="modal-title" id="exampleModalCenterTitle">Add ToDo</h5>
          <button type="button" class="close" data-dismiss="modal" aria-label="Close">
            <span aria-hidden="true">&times;</span>
          </button>
        </div>
        <div class="modal-body">
          <form id="ADDTODO-FORM">
            <div class="form-row">
              <div class="form-group col-md-6">
                <label for="inputName">Name</label>
                <input name="name" type="text" class="form-control" id="inputName" required>
              </div>
              <div class="form-group col-md-6">
              <label for="inputPriority">Priority</label>
                <select name="priority" id="inputPriority" class="form-control">
                  <option>1</option>
                  <option>2</option>
                  <option>3</option>
                  <option>4</option>
                  <option>5</option>
                </select>
              </div>
            </div>
            <div class="form-group">
              <label for="inputDescription">Description</label>
              <input name="description" type="text" class="form-control" id="inputDescription">
            </div>
          </form>
        </div>
        <div class="modal-footer">
          <button type="submit" form="ADDTODO-FORM" value="Submit" class="btn btn-dark">Summit</button>
          <button type="button" data-dismiss="modal" class="btn btn-dark">Cancel</button>
        </div>
      </div>
    </div>
  </div>
  `,
  mounted(){
    var func = this.callback;
    $('#ADDTODO-FORM').on('submit', function(e){
      e.preventDefault();
      var obj = {};
      $('#ADDTODO-FORM').serializeArray().forEach((item, i) => {
        obj[item.name] = item.value;
      });

      if (obj.description === "")
        delete obj.description;

      obj.priority = +obj.priority;
      obj.origin = 'metodo';

      $('#ADDTODO-FORM').trigger("reset");
      func(obj);
    });
  }
})


var alltodos = new Vue({
  el: '#MAIN_COMP',
  data: {
    todos: [],
    toggled: null,
    sortedBy: 0,
    sortableJS: null,
    sync_status: [API.getGITHUB_KEY_STATUS(),API.getGOOGLE_KEY_STATUS(),API.getOUTLOOK_KEY_STATUS(),API.getJWT()],
    interval: null
  },
  async mounted(){
    var todos = await Ipc.get_all_todos();
    this.todos = sortByNormal(todos);
    //console.log(this.todos);
  },
  methods: {
    toggleInfo: function (id) {
      const todo = this.todos.find(item => item._id === id);
      if (this.toggled !== null && this.toggled._id === id){
        this.toggled = null;
      }
      else{
        this.toggled = todo;
      }
    },
    // botão complete todo main comp
    complete: async function (id){
      if (this.interval !== null)
        clearInterval(this.interval);

      const result = await Ipc.complete_todo_id(id);
      if (result){
        if (this.toggled !== null && this.toggled._id === id){
          this.toggled = null;
        }
        this.todos = this.todos.filter((item) => item._id !== id);
      }
      else{
        alert('ERROR!');
      }

      if(this.interval !== null)
        this.interval = setInterval(this.refresh,10000);
    },
    // botão cancel todo main comp
    cancel: async function (id){
      if (this.interval !== null)
        clearInterval(this.interval);

      const result = await Ipc.cancel_todo_id(id);
      if (result){
        if (this.toggled !== null && this.toggled._id === id){
          this.toggled = null;
        }
        this.todos = this.todos.filter((item) => item._id !== id);
      }
      else{
        alert('ERROR!');
      }

      if(this.interval !== null)
        this.interval = setInterval(this.refresh,10000);
    },
    // para lidar com os botões do todo que está a ser mostrado nos details
    complete_toggle: async function(id){
      if (this.interval !== null)
        clearInterval(this.interval);

      const result = await Ipc.complete_todo_id(id);
      if (result){
        this.toggled = null;
        this.todos = this.todos.filter((item) => item._id !== id);
      }
      else{
        alert('ERROR!');
      }

      if(this.interval !== null)
        this.interval = setInterval(this.refresh,10000);
    },
    // para lidar com os botões do todo que está a ser mostrado nos details
    cancel_toggle: async function(id){
      if (this.interval !== null)
        clearInterval(this.interval);

      const result = await Ipc.cancel_todo_id(id);
      if (result){
        this.toggled = null;
        this.todos = this.todos.filter((item) => item._id !== id);
      }
      else{
        alert('ERROR!');
      }

      if(this.interval !== null)
        this.interval = setInterval(this.refresh,10000);
    },
    //metodo para atualizar a ordem da lista quando old_index é menor que new_index
    rev_DaD_update: async function(old_index,new_index){
      if (this.interval !== null)
        clearInterval(this.interval);

      var new_Array = [];

      this.todos.forEach((item, i) => {
        new_Array.push(item);
      });

      var temp = this.todos[old_index];
      for(var i = old_index; i < new_index && i < this.todos.length; i++)
      {
          new_Array[i] = this.todos[i+1];
      }
      new_Array[new_index] = temp;
      updateIndex(new_Array);

      const res = await Ipc.update_list_index(new_Array);
      if (res)
        this.todos = new_Array;
      else
        console.log('ERROR');

      if(this.interval !== null)
        this.interval = setInterval(this.refresh,10000);
    },
    //metodo para atualizar a ordem da lista quando old_index é maior que new_index
    nor_DaD_update: async function(old_index,new_index){
      if (this.interval !== null)
        clearInterval(this.interval);

      var new_Array = [];
      this.todos.forEach((item, i) => {
        new_Array.push(item);
      });

      var temp = this.todos[old_index];
      for(var i = old_index; i > new_index && i > 0; i--)
      {
        new_Array[i] = this.todos[i-1];
      }
      new_Array[new_index] = temp;
      updateIndex(new_Array);

      const res = await Ipc.update_list_index(new_Array);
      if (res)
        this.todos = new_Array;
      else
        console.log('ERROR');

      if(this.interval !== null)
        this.interval = setInterval(this.refresh,10000);
    },
    verifyProperty: function(data){
      return (typeof data !== 'undefined');
    },
    sortBy: function(type){
      if (this.interval !== null)
        clearInterval(this.interval);

      this.sortedBy = type;
      var new_Array = [];
      if (type === 0){
        this.todos = sortByNormal(this.todos);
        this.sortableJS.option("disabled", false);
      }
      else if (type === 1) {
        this.todos = sortByDate(this.todos);
        this.sortableJS.option("disabled", true);
      }
      else {
        this.todos = sortByOrigin(this.todos);
        this.sortableJS.option("disabled", true);
      }

      if(this.interval !== null)
        this.interval = setInterval(this.refresh,10000);
    },
    timeConversor: function (time){
      const ye = new Intl.DateTimeFormat('en', { year: 'numeric' }).format(time);
      const mo = new Intl.DateTimeFormat('en', { month: 'short' }).format(time);
      const da = new Intl.DateTimeFormat('en', { day: '2-digit' }).format(time);

      return da + ' ' + mo + ' ' + ye;
    },
    // função para atualizar os items de um dado tipo: //0 - Github 1 - Google 2 - Outlook
    sync: async function(type){
      if (this.interval !== null)
        clearInterval(this.interval);

      var length = this.todos.length;
      var new_todos = [];

      if (!this.sync_status[type]){
        //GOOGLE-MODAL
        switch (type) {
          case 0:
            $('#GITHUB-MODAL').modal('show');
            break;
          case 1:
            $('#GOOGLE-MODAL').modal('show');
            break;
          case 2:
            $('#OUTLOOK-MODAL').modal('show');
            break;
          default:
        }
      }
      else{
        switch (type) {
          case 0:
            new_todos = await Ipc.get_git_todos();
            break;
          case 1:
            new_todos = await Ipc.get_google_todos();
            break;
          case 2:
            new_todos = await Ipc.get_outlook_todos();
            break;
          default:
        }

        switch (this.sortedBy) {
          case 0:
            this.todos = sortByNormal(new_todos);
            break;
          case 1:
            this.todos = sortByDate(new_todos);
            break;
          case 2:
            this.todos = sortByOrigin(new_todos);
            break;
          default:
        }
        alert((new_todos.length - length)  + ' new ToDos!');

        if(this.interval !== null)
          this.interval = setInterval(this.refresh,10000);
      }
    },
    handleGoogleModal: async function(key,type){
      if (this.interval !== null)
        clearInterval(this.interval);

      const res = await Ipc.store_google_api_key(key);

      if (res !== true){
        alert('Error saving Google API key');
      }
      else{
        this.sync_status[type] = true;
        alert('Google API key validated');
      }
      $('#GOOGLE-MODAL').modal('hide');

      if(this.interval !== null)
        this.interval = setInterval(this.refresh,10000);
    },
    handleOutlookModal: async function(){
      if (this.interval !== null)
        clearInterval(this.interval);

      const res = await Ipc.verify_Outlook_Key();
      console.log(res);

      if (res !== true){
        alert('Error saving Outlook API key');
      }
      else{
        this.sync_status[2] = true;
        alert('Outlook API key validated');
      }

      $('#OUTLOOK-MODAL').modal('hide');

      if(this.interval !== null)
        this.interval = setInterval(this.refresh,10000);
    },
    handleGitHubModal: async function (){
      if (this.interval !== null)
        clearInterval(this.interval);

      const res = await Ipc.verify_Github_Key();
      console.log(res);

      if (res !== true){
        alert('Error saving Github API key');
      }
      else {
        this.sync_status[0] = true;
        alert('Github API key validated');
      }

      $('#GITHUB-MODAL').modal('hide');

      if(this.interval !== null)
        this.interval = setInterval(this.refresh,10000);
    },
    handleLogInModal: function (){
      if (!this.sync_status[3]){
        $('#LOGIN-MODAL').modal('show');
      }
    },
    // função que mostra o modal
    toggleAddTodo: function(){
      $('#ADD_TOO_COMP').modal('show');
    },
    // função para adicionar um todo.
    addTodo: async function (obj){
      if (this.interval !== null)
        clearInterval(this.interval);

      const res = await Ipc.add_todo(obj);

      if (typeof res !== 'undefined'){
        this.todos.push(res);
        switch (this.sortedBy) {
          case 0:
            this.todos = sortByNormal(this.todos);
            break;
          case 1:
            this.todos = sortByDate(this.todos);
            break;
          case 2:
            this.todos = sortByOrigin(this.todos);
            break;
          default:
        }
        alert('Todo Added');
      }
      $('#ADD_TOO_COMP').modal('hide');

      if (this.interval !== null)
        this.interval = setInterval(this.refresh,10000);
    },
    handleLogInSubmit: async function(type,email,pass){
      if (type){
        const resp = await Ipc.login(email,pass);
        switch (resp) {
          case 200:
            $('#LOGIN-MODAL').modal('hide');
            alert('Logged in');
            //this.sync_status[3] = true;
            //this.sync_status = [...this.sync_status];
            this.sync_status = [API.getGITHUB_KEY_STATUS(),API.getGOOGLE_KEY_STATUS(),API.getOUTLOOK_KEY_STATUS(),true];
            this.interval = setInterval(this.refresh,10000);
            break;
          case 409:
            alert('Wrong credentials');
            break;
          default:
            alert('An error was found');
            break;
        }
      }
      else{
        const resp = await Ipc.register(email,pass);
        switch (resp) {
          case 200:
            $('#LOGIN-MODAL').modal('hide');
            alert('Account created with success');
            break;
          case 409:
            alert('Email is already in use');
            break;
          default:
            alert('An error was found');
            break;
        }
      }
    },
    toggleSync: async function (){
      if (this.interval !== null)
        clearInterval(this.interval);

      var res = await Ipc.sync();
      if (res !== null){
        alert('Sync Completed');
      }
      else{
        alert('Error on sync');
      }

      if (this.interval !== null)
        this.interval = setInterval(this.refresh,10000);
    },
    refresh: async function (){
      var size = this.todos.length;
      this.todos = await Ipc.get_all_todos();
      this.sortBy(this.sortedBy);
      if (this.todos.length > size){
        alert((this.todos.length - size)  + ' new ToDos!');
      }
    },
    test: function(){
      alert('Wele');
    },
  },
})


var el = document.getElementById('cards')
alltodos.sortableJS = Sortable.create(el,{
  animation: 150,
  onEnd: function (evt){
    if (evt.oldDraggableIndex < evt.newDraggableIndex){
      alltodos.rev_DaD_update(evt.oldDraggableIndex,evt.newDraggableIndex);
    }
    else if (evt.oldDraggableIndex > evt.newDraggableIndex) {
      alltodos.nor_DaD_update(evt.oldDraggableIndex,evt.newDraggableIndex);
    }
  },
});


/**
window.ipcRenderer.on('testing',(event,arg) => {
  console.log(arg);
})

window.ipcRenderer.send('test');*/

//sortable.option("disabled", true);

console.log('👋 This message is being logged by "renderer.js", included via webpack');
