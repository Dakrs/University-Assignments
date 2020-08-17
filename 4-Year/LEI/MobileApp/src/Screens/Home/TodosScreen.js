/**
 * Sample React Native App
 * https://github.com/facebook/react-native
 *
 * @format
 * @flow strict-local
 */

import React,{useState, useEffect}  from 'react';
import {
  SafeAreaView,
  StyleSheet,
  ScrollView,
  View,
  StatusBar,
} from 'react-native';

import NavBar from '../../Components/NavBar';
import MyCard from '../../Components/Card'
import { Provider as PaperProvider} from 'react-native-paper';

import {sortByOrigin,sortByNormal,sortByDate} from '../../Sorting'



 function TodosScreen({navigation,Todos,completeTodo,cancelTodo}){

  const[todos,setTodos] = useState([])


  useEffect(() => {
      setTodos(Todos)
  },[])


  const SortByOrigin = () =>{
    var tod = sortByOrigin(todos)
      setTodos(tod)
  }

  const SortByNormal = () =>{
    var tod = sortByNormal(todos)
      setTodos(tod)
  }

  const SortByDate = () =>{
    var tod = sortByDate(todos)
      setTodos(tod)
  }

  

    



  return (
    <PaperProvider style={styles.font}>
      <StatusBar barStyle="dark-content" />
      <SafeAreaView style={{flex: 1, flexDirection:'column'}} >
      <NavBar style={{flex: 1}} SortByOrigin={SortByOrigin} SortByNormal={SortByNormal} 
              SortByDate={SortByDate}/>
      <ScrollView style={{flex:1} }>
        {todos.map((todo) =>{
          return(
            <MyCard key={todo._id} navigation={navigation} todoInfo={todo} completeTodo={completeTodo} cancelTodo={cancelTodo}></MyCard>
          )
        })}
      </ScrollView>
      </SafeAreaView>
    </PaperProvider>
  );
};

const styles = StyleSheet.create({
  font: {
    fontFamily:"Helvetica, Arial, sans-serif"
  }
})

export default TodosScreen;
