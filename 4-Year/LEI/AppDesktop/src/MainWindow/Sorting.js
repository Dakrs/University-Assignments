export function sortByOrigin(list){
  var new_Array = [];
  var groupby = new Map();
  list.forEach((item, i) => {
    if (!groupby.has(item.origin))
      groupby.set(item.origin,[])
    groupby.get(item.origin).push(item);
  });
  for (const [key,value] of groupby.entries()){
    value.forEach((item, i) => {
      new_Array.push(item);
    });
  }

  return new_Array;
}

export function sortByDate(list){
  var new_Array = [];
  list.forEach((item, i) => {
    new_Array.push(item);
  });
  new_Array.sort((a,b) => {
    if (!a.hasOwnProperty('date')){
      return 1;
    }
    if (!b.hasOwnProperty('date')){
      return -1;
    }

    return (a.date.getTime() - b.date.getTime());
  });

  return new_Array;
}

export function sortByNormal(list){
  var new_Array = [];

  const [p_index, f_index] = list.reduce( ([p,f], e) => (e.hasOwnProperty('index') ? [[...p,e],f] : [p,[...f,e]]), [[],[]] );
  const [p_date, f_date] = f_index.reduce( ([p,f], e) => (e.hasOwnProperty('date') ? [[...p,e],f] : [p,[...f,e]]), [[],[]] );

  p_index.sort((a,b) => (a.index - b.index));
  p_date.sort((a,b) => (a.date.getTime() - b.date.getTime()));
  f_date.sort((a,b) => (a.name.localeCompare(b.name)));

  p_index.forEach((item, i) => {
    new_Array.push(item);
  });
  p_date.forEach((item, i) => {
    new_Array.push(item);
  });
  f_date.forEach((item, i) => {
    new_Array.push(item);
  });
  return new_Array;
}

export function updateIndex(list){
  list.forEach((item, i) => {
    item.index = i;
  });
}

export default {sortByOrigin, sortByDate, sortByNormal, updateIndex};
