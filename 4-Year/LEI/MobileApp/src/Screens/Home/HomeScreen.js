import TodosScreen from './TodosScreen';
import DetailsScreen from './DetailsScreen';


import { createStackNavigator } from '@react-navigation/stack';
import React from 'react';

Stack = createStackNavigator()


function HomeScreen({Todos,completeTodo,cancelTodo}){
  // rever o props porque nao é boa cena ser
        return(
    <Stack.Navigator
    initialRouteName="TodosScreen"
    >
    <Stack.Screen
      name="TodosScreen"
      options={
        {headerShown:false}}>
      {props => <TodosScreen{...props} Todos={Todos} completeTodo={completeTodo} cancelTodo={cancelTodo}/> }
    </Stack.Screen>
     <Stack.Screen
      name="Details"
      component={DetailsScreen}
      
    />
    </Stack.Navigator>
    )
}
export default HomeScreen