import * as React from 'react';

import { Menu,Divider, Button,TextInput,} from 'react-native-paper';
import {
  Alert,
  Modal,
  StyleSheet,
  Text,
  View,
  Linking,
  TouchableOpacity
} from "react-native";

import api from '../../Services/api'

import { getUser } from '../../utils'

export default function Sync({syncGoogle,syncOutlook,syncGithub}){

  const [modalVisible1, setModalVisible1] = React.useState(false);
  const [modalVisible2, setModalVisible2] = React.useState(false);
  const [url,setUrl] = React.useState()
  const [code,setCode] = React.useState('')
  const [type,setType] =React.useState('')

  _closeModal2 = () => setModalVisible2(false);
  _closeModal1 = () => setModalVisible1(false)
  
async function checkCredentials(type){
  setType(type)
  var user= await getUser()
  user=JSON.parse(user)
  var resp= await api.get('/'+type+'/credentials')
  console.log(resp.data)
    if (resp.data.length > 0) {
       if( type === "google"){
          await syncGoogle()
       }
       else if (type==="outlook"){
         await syncOutlook() 
       }

       else if (type==="github"){
         await syncGithub()

       }
        Alert.alert("You are sync :)")
    }
    else {
      var url = await api.get('/'+type+'/url')
        setUrl(url.data)
        
        if(type==="google")
        setModalVisible1(true)
        else
        setModalVisible2(true)
        console.log(url.data)
    }
  }
  
  async function sendCode(){
    var user= await getUser()
  user=JSON.parse(user)
  var resp= await api.post('/'+type+'/code',{code:code,owner:user.user})
  if(!resp.data)
  Alert.alert("Error in code\n Try Again")
  }

    return(
      <View>
          <Menu.Item icon='github-circle' onPress={() => { ;checkCredentials("github") } } title="Github" />
          <Divider/>
          <Menu.Item  icon='google'onPress={() => { ;checkCredentials("google") }} title="Google" />
          <Divider/>
          <Menu.Item  icon= 'microsoft' onPress={() => {;checkCredentials("outlook") } } title="Outlook" /> 
      

          <Modal
              style={{flex:1}}
              animationType="fade"
              transparent={true}
              visible={modalVisible2}>
                
              <TouchableOpacity
                  style={styles.shadow}
                  activeOpacity={1}
                  onPressOut={() => _closeModal2()}>
                  
                  <View style={styles.container}>
                    <Text style = {{marginLeft:"5%"}}>In order to access and colect usefull data for your application we need to access some sources</Text>
                    <Button  style={styles.button} mode="contained" onPress={() => { Linking.openURL(url) ; setModalVisible2(false)} }>
                       <Text style={{fontSize:6}}> Link to generate API Key</Text> 
                    </Button>

                  </View>

              </TouchableOpacity>
                
              
          </Modal>

          <Modal
              style={{flex:1}}
              animationType="fade"
              transparent={true}
              visible={modalVisible1}>
             <TouchableOpacity
                  style={styles.shadow}
                  activeOpacity={1}
                  onPressOut={() => _closeModal1()}>
                  
                  <View style={styles.container}>
                    <Text style = {{marginLeft:"5%"}}>In order to access and colect usefull data for your application we need to access some sources</Text>
                    <Button  style={styles.button} mode="contained" onPress={() => { Linking.openURL(url) ; setModalVisible2(false)} }>
                       <Text style={{fontSize:6}}> Link to generate API Key</Text> 
                    </Button>
                    <TextInput
                        label="Code"
                        value={code}
                        onChangeText={code => setCode(code)}/>
                    <Button style={styles.button} mode="contained" onPress={() =>  { sendCode(); setModalVisible1(false)}}>
                    <Text style={{fontSize:6}}> Submit</Text> 
                    </Button>
                  </View>

              </TouchableOpacity>
                   
      </Modal>

      </View>
    

    )
}


  const styles = StyleSheet.create({
  
    shadow: {
      flex:1,
      justifyContent:'center',
      backgroundColor:"grey",

    },
    button: {
      backgroundColor:'rgb(0,0.2,1.0)',
      margin:"auto",
      justifyContent:"center",
      marginLeft:"20%",
      marginRight:"20%",
      marginTop: "5%",
      marginBottom:"5%",
    },
    container:{
      marginLeft:"5%",
      marginRight:"5%",
      backgroundColor:"white"
    }
  });
