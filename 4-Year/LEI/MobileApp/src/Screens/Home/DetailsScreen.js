
import React from 'react';
import {
  StyleSheet,
  View,
} from 'react-native';

import {Card,
  Title,
  Paragraph,
  Button} from 'react-native-paper';
  import { Icon, Divider } from 'react-native-elements'
  import 'intl';
  import 'intl/locale-data/jsonp/en';
   

  function parseDate(str) {
    var m = str.match(/[^T]+/);
      
    return m 
  }
 
const DetailsScreen = ({navigation ,route})=> {

  const {todoInfo}= route.params
  //const {todoInfo,completeTodo,cancelTodo}= route.params
  /*
  _complete = () => {
    completeTodo(todoInfo._id)
  }
  _cancel = () => {
    cancelTodo(todoInfo._id)
  }
  
 */
  function timeConversor(time){
    const ye = new Intl.DateTimeFormat('en', { year: 'numeric' }).format(time);
    const mo = new Intl.DateTimeFormat('en', { month: 'short' }).format(time);
    const da = new Intl.DateTimeFormat('en', { day: '2-digit' }).format(time);

    return da + ' ' + mo + ' ' + ye;
  }  


  return (
      <Card style={styles.card}>
        <Card.Content style={{marginBottom: 10}}>
          <Title>{todoInfo.name}</Title>
            {todoInfo.description ? (
          <View>
            <Title  style={{fontSize: 14}}>Description:</Title>
            <Paragraph style={{color:'grey'}}>
              {todoInfo.description}
            </Paragraph>
          </View>
            ) : (
          <View></View> 
            )}

        </Card.Content>
        <Card.Content style={{marginBottom: 10}}>      
          <Title style={{fontSize: 14}}>Details:</Title>  
          <View style={styles.row}>
            <Paragraph style={{color:'grey'}}>Priority</Paragraph>
            <Paragraph>{todoInfo.priority}</Paragraph>
          </View>

          {todoInfo.date ? (
            <View style={styles.row}>
              <Paragraph style={{color:'grey'}} >Expire Date</Paragraph>
              <Paragraph>{timeConversor(todoInfo.date)}</Paragraph>
            </View>
          ):(
            <View></View>
          )}

          <View style={styles.row}>
            <Paragraph style={{color:'grey'}}>Origin</Paragraph>
            <Paragraph>{todoInfo.origin}</Paragraph>       
          </View>
        </Card.Content>
        <Divider></Divider>
        
      </Card>
    )}
 


    const styles = StyleSheet.create({
        card: {
          flex:1,
          marginLeft: 25,
          marginRight:25,
          marginBottom:10,
          marginTop:10,
          

        },
        row:{
          flexDirection: "row",
          justifyContent: "space-between",
          alignItems: "center"
          
        },

        button:{
          margin:10,
          height: 25, 
          width: 100,
          fontSize:10,
          backgroundColor:"white",
          borderColor:'green',
          borderRadius:4
          },

        buttons:{
          flex:1,
          flexDirection:'row',
          justifyContent:'space-between'
      }
      
      }); 
      
/*
      <Card.Content style={styles.row}>
      <Button 
      labelStyle={{fontSize:10,color:'green',}} 
      style={styles.button} 
      mode="Text" 
      onPress={() => {_complete(); navigation.push('TodosScreen')} }>
    Complete
      </Button> 
      <Button 
      labelStyle={{fontSize:10,color:'red',}} 
      style={styles.button} 
      mode="Text" 
      onPress={() => {_cancel(); navigation.push('TodosScreen') }}>
    Cancel
      </Button>   
    </Card.Content> 
      
  */
     export default DetailsScreen;
