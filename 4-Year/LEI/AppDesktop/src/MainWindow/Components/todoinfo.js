import Vue from 'vue';

/**
function testing() {
  alert('Hello');
}

module.exports = testing*/

Vue.component('todo-info',{
  props: {
    todo: Object,
    callback_completed: Function,
    callback_cancel: Function,
  },
  template: `
  <div class="card mb-3 nomove">
    <div class="card-body" style="padding: 15px; padding-bottom:10px;">
      <h5 class="card-title" style="margin:0;margin-bottom: 0.75rem;text-align: center;">{{ todo.name }}</h5>
      <template v-if="verifyProperty(todo.description)">
        <span>Description:</span>
        <p class="text-muted justify">{{ todo.description }}</p>
      </template>
      <span>Details:</span>
      <table class="table table-borderless" style="margin-bottom:0">
        <tbody>
          <tr v-if="verifyProperty(todo.priority)">
            <td class="details text-left grey">Priority</td>
            <td class="details text-right">{{ todo.priority }}</td>
          </tr>
          <tr v-if="verifyProperty(todo.date)">
            <td class="details text-left grey">Expire Date</td>
            <td class="details text-right">{{ timeConv(todo.date) }}</td>
          </tr>
          <tr v-if="verifyProperty(todo.origin)">
            <td class="details text-left grey">Origin</td>
            <td class="details text-right">{{ todo.origin }}</td>
          </tr>
        </tbody>
      </table>
    </div>
    <div class="card-footer text-muted specfooter" align="center">
      <div class="btn-group" role="group" aria-label="Basic example">
        <button type="button" @click="complete()" class="btn btn-outline-success">Complete</button>
        <button type="button" @click="cancel()" class="btn btn-outline-danger">Cancel</button>
      </div>
    </div>
  </div>
  `,
  methods: {
    complete: function (){
      this.callback_completed(this.todo._id);
    },
    cancel: function (){
      this.callback_cancel(this.todo._id);
    },
    verifyProperty: function(data){
      return (typeof data !== 'undefined');
    },
    timeConv: function (time){
      const ye = new Intl.DateTimeFormat('en', { year: 'numeric' }).format(time);
      const mo = new Intl.DateTimeFormat('en', { month: 'short' }).format(time);
      const da = new Intl.DateTimeFormat('en', { day: '2-digit' }).format(time);

      return da + ' ' + mo + ' ' + ye;
    }
  }
})
